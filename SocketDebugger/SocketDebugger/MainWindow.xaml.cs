using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

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

        private void BindPort_Click(object sender, RoutedEventArgs e)
        {
            Grid grid = Content as Grid;
            TextBox port_box = grid.FindName("Port") as TextBox;
            
        }

        private void CleanRecvBox_Click(object sender, RoutedEventArgs e)
        {

        }

        private void SendMessages_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
