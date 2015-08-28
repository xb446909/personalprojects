using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
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

namespace UdpCommunication
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
        Rect normal_rect;
        Brush default_brush = new SolidColorBrush(Color.FromRgb(0xcc, 0xcc, 0xcc));//new SolidColorBrush(Color.FromRgb(83, 92, 168));

        RegClient m_regclient;
        Setting setting;

        int port = 10000;
        string host = "localhost";
        int interval_s = 10;

        Socket so = new Socket(SocketType.Dgram, ProtocolType.Udp);

        public MainWindow()
        {
            InitializeComponent();

            Grid MainGrid = Content as Grid;
            restore = MainGrid.FindName("Restore") as Canvas;
            maximize = MainGrid.FindName("Maximize") as Canvas;
            minimize = MainGrid.FindName("Minimize") as Canvas;
            restore.Visibility = Visibility.Collapsed;
            Background = default_brush;

            m_regclient = new RegClient(interval_s);
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
            Left = 0;
            Top = 0;
            Width = rect.Width;
            Height = rect.Height;

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

        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.OriginalSource is Border)
            {
                WindowInteropHelper wih = new WindowInteropHelper(this);
                SendMessage(wih.Handle, 0x00A1, 2, 0);
                return;
            }
        }

        private void Setting_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            setting = new Setting();
            setting.Text_Port.Text = port.ToString();
            setting.Text_Server.Text = host;
            setting.Owner = this;
            if (setting.ShowDialog() == true)
            {
                port = Convert.ToInt32(setting.Text_Port.Text);
                host = setting.Text_Server.Text;
            }
        }
    }
}
