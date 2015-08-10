using System;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;

namespace SocketDebugger
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Sends the specified message to a window or windows  
        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);

        Canvas restore;
        Canvas maximize;
        Canvas minimize;
        TabControl tab_ctrl;
        TabItem tcp_server;
        TabItem tcp_client;
        TabItem udp_server;
        TabItem udp_client;

        Rect normal_rect;

        Brush default_brush = new SolidColorBrush(Color.FromRgb(83, 92, 168));
        Brush brush_color1 = new SolidColorBrush(Color.FromRgb(241, 141, 0));
        Brush brush_color2 = new SolidColorBrush(Color.FromRgb(255, 238, 125));
        Brush brush_color3 = new SolidColorBrush(Color.FromRgb(168, 209, 118));
        Brush brush_color4 = new SolidColorBrush(Color.FromRgb(232, 68, 120));

        TcpServerDebug m_TcpServer;
        TcpClientDebug m_TcpClient;
        UdpServerDebug m_UdpServer;
        UdpClientDebug m_UdpClient;

        public MainWindow()
        {
            InitializeComponent();
            Grid MainGrid = Content as Grid;
            restore = MainGrid.FindName("Restore") as Canvas;
            maximize = MainGrid.FindName("Maximize") as Canvas;
            minimize = MainGrid.FindName("Minimize") as Canvas;
            tab_ctrl = MainGrid.FindName("tabControl") as TabControl;
            tcp_server = MainGrid.FindName("TCPServer") as TabItem;
            tcp_client = MainGrid.FindName("TCPClient") as TabItem;
            udp_server = MainGrid.FindName("UDPServer") as TabItem;
            udp_client = MainGrid.FindName("UDPClient") as TabItem;
            restore.Visibility = Visibility.Collapsed;
            Background = default_brush;
            tcp_server.Background = brush_color1;
            tcp_client.Background = brush_color2;
            udp_server.Background = brush_color3;
            udp_client.Background = brush_color4;

            m_TcpServer = new TcpServerDebug(MainGrid);
            m_TcpClient = new TcpClientDebug(MainGrid);
            m_UdpServer = new UdpServerDebug(MainGrid);
            m_UdpClient = new UdpClientDebug(MainGrid);

        }

        private void OnCloseWindow(object sender, MouseButtonEventArgs e)
        {
            Close();
        }

        private void OnMouseEnter(object sender, MouseEventArgs e)
        {
            Canvas can = sender as Canvas;
            can.Background = Brushes.White;
        }

        private void OnMouseLeave(object sender, MouseEventArgs e)
        {
            Canvas can = sender as Canvas;
            can.Background = default_brush;
        }

        private void Maximize_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            maximize.Visibility = Visibility.Collapsed;
            restore.Visibility = Visibility.Visible;
            normal_rect = new Rect(Left, Top, Width, Height);

            Rect rect = SystemParameters.WorkArea;
            Left = -5;
            Top = -5;
            Width = rect.Width + 10;
            Height = rect.Height + 10;

            tab_ctrl.Width = Width;
            tab_ctrl.Height = Height;
        }

        private void Minimize_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void Restore_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            restore.Visibility = Visibility.Collapsed;
            maximize.Visibility = Visibility.Visible;

            Left = normal_rect.Left;
            Top = normal_rect.Top;
            Width = normal_rect.Width;
            Height = normal_rect.Height;

            tab_ctrl.Width = Width;
            tab_ctrl.Height = Height;
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.OriginalSource is Border || e.OriginalSource is System.Windows.Controls.Primitives.TabPanel)
            {
                WindowInteropHelper wih = new WindowInteropHelper(this);
                SendMessage(wih.Handle, 0x00A1, 2, 0);
                return;
            }
        }

        private void TcpServerBindPort_Click(object sender, RoutedEventArgs e)
        {
            Grid grid = Content as Grid;
            TextBox port_box = grid.FindName("Port") as TextBox;
            Button btn = sender as Button;

            if(btn.Content.ToString() == "断开连接")
            {
                m_TcpServer.TcpServerStopConnect();
                btn.Content = "监听端口";
                return;
            }


            int port_num;
            if (int.TryParse(port_box.Text, out port_num) == false)
            {
                MessageBox.Show("请输入正确的端口号");
                return;
            }

            btn.Content = "断开连接";
            m_TcpServer.TcpServerBind(port_num);
        }

        private void TcpServerCleanRecvBox_Click(object sender, RoutedEventArgs e)
        {
            m_TcpServer.recv_box.Text = "";
        }

        private void TcpServerSendMessages_Click(object sender, RoutedEventArgs e)
        {
            m_TcpServer.TcpServerSend(m_TcpServer.send_box.Text);
            m_TcpServer.send_box.Text = "";
        }

        private void TcpClientConnect_Click(object sender, RoutedEventArgs e)
        {
            Grid grid = Content as Grid;
            TextBox server_addr = grid.FindName("TcpServerAddr") as TextBox;
            TextBox port_box = grid.FindName("TcpServerPort") as TextBox;
            Button btn = (Button)sender;

            if (btn.Content.ToString() == "断开")
            {
                m_TcpClient.TcpClientDisconnect();
                btn.Content = "连接";
                return;
            }

            int port_num;
            if (int.TryParse(port_box.Text, out port_num) == false)
            {
                MessageBox.Show("请输入正确的端口号");
                return;
            }
            btn.Content = "断开";
            m_TcpClient.TcpClientConnect(server_addr.Text, port_num);
        }

        private void TcpClientCleanRecvBox_Click(object sender, RoutedEventArgs e)
        {
            m_TcpClient.recv_box.Text = "";
        }

        private void TcpClientSendMessages_Click(object sender, RoutedEventArgs e)
        {
            m_TcpClient.TcpClientSend(m_TcpClient.send_box.Text);
            m_TcpClient.send_box.Text = "";
        }

        private void UdpServerBindPort_Click(object sender, RoutedEventArgs e)
        {
            Grid grid = Content as Grid;
            TextBox port_box = grid.FindName("UdpServerPort") as TextBox;
            Button btn = sender as Button;

            if (btn.Content.ToString() == "断开连接")
            {
                btn.Content = "绑定端口";
                m_UdpServer.UdpServerClose();
                return;
            }


            int port_num;
            if (int.TryParse(port_box.Text, out port_num) == false)
            {
                MessageBox.Show("请输入正确的端口号");
                return;
            }

            btn.Content = "断开连接";
            m_UdpServer.UdpServerBind(port_num);
        }

        private void UdpServerCleanRecvBox_Click(object sender, RoutedEventArgs e)
        {
            //m_UdpServer.recv_box.Document.Blocks.Clear();
            m_UdpServer.p.Inlines.Clear();
        }

        private void UdpServerSendMessages_Click(object sender, RoutedEventArgs e)
        {
            m_UdpServer.UdpServerSend(m_UdpServer.send_box.Text);
            m_UdpServer.send_box.Text = "";
        }

        private void UdpClientCleanRecvBox_Click(object sender, RoutedEventArgs e)
        {
            m_UdpClient.recv_box.Text = "";
        }

        private void UdpClientSendMessages_Click(object sender, RoutedEventArgs e)
        {
            Grid grid = Content as Grid;
            TextBox server_addr = grid.FindName("UdpClientIPAddress") as TextBox;
            TextBox port_box = grid.FindName("UdpClientPort") as TextBox;
            int port_num;
            if (int.TryParse(port_box.Text, out port_num) == false)
            {
                m_UdpClient.recv_box.Text += "请输入正确的端口号\r\n";
                return;
            }
            m_UdpClient.UdpClientSend(server_addr.Text, port_num, m_UdpClient.send_box.Text);
            m_UdpClient.send_box.Text = "";
        }
    }

    internal class StateObject
    {
        public const int BUFFER_SIZE = 1024;
        public byte[] buffer = new byte[BUFFER_SIZE];
        public Socket workSocket = null;
        public Thread thread;
        public TcpListener listener;
        public TcpClient client;
        public IAsyncResult ar;
    }
}
