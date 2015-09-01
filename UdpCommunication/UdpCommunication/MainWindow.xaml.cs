using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Threading;

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

        SendClient m_sendclient;
        NameWindow m_name;
        RegClient m_regclient;
        Setting setting;

        int port = 10000;
        string host = "localhost";
        int interval_s = 10;
        string name;

        Thread recv_thread;
        static Socket socket;
        static IPEndPoint endpoint;

        static List<ClientInfo> clientlst = new List<ClientInfo>();
        static ListBox lstbox;


        public MainWindow()
        {
            InitializeComponent();

            Grid MainGrid = Content as Grid;
            restore = MainGrid.FindName("Restore") as Canvas;
            maximize = MainGrid.FindName("Maximize") as Canvas;
            minimize = MainGrid.FindName("Minimize") as Canvas;
            restore.Visibility = Visibility.Collapsed;
            Background = default_brush;

            m_sendclient = new SendClient(interval_s);
            m_regclient = new RegClient(interval_s, ClientList);
            recv_thread = new Thread(Receive_Thread);

            socket = new Socket(SocketType.Dgram, ProtocolType.Udp);

            lstbox = ClientList;
            lstbox.Items.Clear();
        }

        private static void Receive_Thread(object param)
        {
            EndPoint ep = (EndPoint)endpoint;
            IPEndPoint temp = new IPEndPoint(0, 0);
            byte[] buf = new byte[128];
            int total, num, len;
            while (true)
            {
                num = 0;
                len = 0;
                try
                {
                    len = socket.ReceiveFrom(buf, ref ep);
                    string str = Encoding.Default.GetString(buf, 0, len);
                    string[] words = Regex.Split(str, "#");
                    if (words.Length == 1)
                    {
                        continue;
                    }
                    if (words[1].Equals("GOT"))
                    {
                        MessageBox.Show("GOT");
                        continue;
                    }
                    if (words[1].Equals("MSG"))
                    {
                        if (words[2].Equals("SND"))
                        {
                            temp.Address = IPAddress.Parse(words[3]);
                            temp.Port = int.Parse(words[4]);
                            socket.SendTo(Encoding.Default.GetBytes("#GOT#"), temp);
                        }
                        else
                        {
                            socket.SendTo(Encoding.Default.GetBytes("#GOT#"), ep);
                        }
                        MessageBox.Show("Send GOT to: " + ((IPEndPoint)ep).Address.ToString() + ":" + ((IPEndPoint)ep).Port.ToString());
                        continue;
                    }
                    if (words[1].Equals("LST"))
                    {
                        if (int.TryParse(words[2], out total) == false)
                        {
                            continue;
                        }
                        if (total == 0)
                        {
                            Dispatcher.FromThread((Thread)param).Invoke(new Action(() =>
                            {
                                lstbox.ItemsSource = null;
                            }));
                            clientlst.Clear();
                            Dispatcher.FromThread((Thread)param).Invoke(new Action(() =>
                            {
                                lstbox.ItemsSource = clientlst;
                            }));
                            continue;
                        }
                        if (int.TryParse(words[3], out num) == false)
                        {
                            continue;
                        }
                        if (num == 1)
                        {
                            Dispatcher.FromThread((Thread)param).Invoke(new Action(() =>
                            {
                                lstbox.ItemsSource = null;
                            }));
                            clientlst.Clear();
                        }
                        clientlst.Add(new ClientInfo(words[4]));
                        if (total == num)
                        {
                            Dispatcher.FromThread((Thread)param).Invoke(new Action(() =>
                            {
                                lstbox.ItemsSource = clientlst;
                            }));
                        }
                        continue;
                    }
                }
                catch (System.Exception e)
                {
                    //MessageBox.Show(e.Message.ToString());
                    //return;
                }
            }
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


                m_regclient.StopFlap();
                socket.Close();
                recv_thread.Abort();

                socket = new Socket(SocketType.Dgram, ProtocolType.Udp);

                int index = -1;
                var address = Dns.GetHostEntry(host);
                for (int i = 0; i < address.AddressList.Length; i++)
                {
                    if (address.AddressList[i].AddressFamily == AddressFamily.InterNetwork)
                    {
                        index = i;
                        break;
                    }
                }
                if (index == -1)
                {
                    MessageBox.Show("无法从" + host + "获得IP地址.");
                    return;
                }

                endpoint = new IPEndPoint(address.AddressList[index], port);
                socket.Connect(endpoint);

                m_regclient.StartFlap(socket, endpoint);
                recv_thread = new Thread(Receive_Thread);
                recv_thread.Start(Thread.CurrentThread);
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            m_name = new NameWindow();
            m_name.Owner = this;
            if (m_name.ShowDialog() == true)
            {
                name = m_name.name;
                label_welcome.Content = "Welcome " + name + " !";
            }
            m_regclient.name = name;

            int index = -1;
            var address = Dns.GetHostEntry(host);
            for (int i = 0; i < address.AddressList.Length; i++)
            {
                if (address.AddressList[i].AddressFamily == AddressFamily.InterNetwork)
                {
                    index = i;
                    break;
                }
            }
            if (index == -1)
            {
                MessageBox.Show("无法从" + host + "获得IP地址.");
                return;
            }

            endpoint = new IPEndPoint(address.AddressList[index], port);
            socket.Connect(endpoint);

            m_regclient.StartFlap(socket, endpoint);
            recv_thread.Start(Thread.CurrentThread);

        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            socket.Close();
            recv_thread.Abort();
        }

        private void TextBlock_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            TextBlock block = sender as TextBlock;
            string text = block.Text;
            ClientInfo info = new ClientInfo(text);

            m_sendclient.StartSend(socket, info.endpoint);
            socket.SendTo(Encoding.Default.GetBytes("#MSG#" + info.info), endpoint);
        }
    }

    internal class ClientInfo 
    {
        string[] words;
        public string info { get; set; }
        public string name
        {
            get
            {
                return words[0];
            }
        }
        public string ip
        {
            get
            {
                return words[1];
            }
        }
        public string port
        {
            get
            {
                return words[2];
            }
        }
        public IPEndPoint endpoint
        {
            get
            {
                return new IPEndPoint(IPAddress.Parse(ip), int.Parse(port));
            }
        }
        public ClientInfo(string client_info)
        {
            info = client_info;
            words = Regex.Split(info, "[\\ :]");
        }
    }
}
