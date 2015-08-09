using System;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace SocketDebugger
{
    internal class TcpClientDebug
    {
        public TextBox recv_box;
        public TextBox send_box;
        public Ellipse led;
        private TcpClient server;
        private string remote_host;
        private int remote_port;
        private StateObject so = new StateObject();

        public TcpClientDebug(object obj)
        {
            Grid main_grid = obj as Grid;
            recv_box = main_grid.FindName("TcpClientReceiveControl") as TextBox;
            send_box = main_grid.FindName("TcpClientSendControl") as TextBox;
            led = main_grid.FindName("TcpClientStateLed") as Ellipse;
        }

        public void TcpClientConnect(string host, int port)
        {
            server = new TcpClient();
            so.thread = Thread.CurrentThread;
            so.client = server;
            try
            {
                server.BeginConnect(host, port, new AsyncCallback(ConnectCallback), so);
            }
            catch (Exception e)
            {
                recv_box.Text += e.Message + "\r\n";
                recv_box.ScrollToEnd();
            }
            led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
            remote_host = host;
            remote_port = port;
        }

        public void TcpClientDisconnect()
        {
            if (so.client != null)
            {
                try
                {
                    if (!so.client.Client.Connected)
                    {
                        if (so.ar != null)
                        {
                            so.client.Client.EndConnect(so.ar);
                        }
                        so.client.Client.Close();
                    }
                    else
                    {
                        so.workSocket.Disconnect(false);
                        so.workSocket.Close();
                        so.workSocket.Dispose();
                    }
                    so.client = null;
                }
                catch (SocketException e)
                {
                    recv_box.Text += e.Message + "\r\n";
                }

                led.Fill = new SolidColorBrush(Color.FromRgb(200, 0, 0));
            }
        }

        public void ConnectCallback(IAsyncResult ar)
        {
            StateObject so = (StateObject)ar.AsyncState;
            if (so.client == null) return;
            so.ar = ar;
            so.workSocket = so.client.Client;
            if (so.workSocket.Connected == false)
            {
                try
                {
                    so.client.BeginConnect(remote_host, remote_port, new AsyncCallback(ConnectCallback), so);
                }
                catch (Exception e)
                {
                    Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                    {
                        led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
                        recv_box.Text += e.Message + "\r\n";
                        recv_box.ScrollToEnd();
                    }));
                }
                return;                
            }
            Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
            {
                led.Fill = new SolidColorBrush(Color.FromRgb(0, 200, 0));
            }));
            
            try
            {
                so.workSocket.BeginReceive(so.buffer, 0, StateObject.BUFFER_SIZE, 0, new AsyncCallback(ReceiveCallback), so);
            }
            catch (Exception e)
            {
                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    recv_box.Text += e.Message + "\r\n";
                    recv_box.ScrollToEnd();
                    led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
                }));
            }
        }


        public void ReceiveCallback(IAsyncResult ar)
        {
            StateObject so = (StateObject)ar.AsyncState;
            so.ar = ar;
            Socket s = so.workSocket;

            int read = 0;

            try
            {
                read = s.EndReceive(ar);
            }
            catch (Exception e)
            {
                if (e.HResult == -2147467259)
                {
                    Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                    {
                        recv_box.Text += e.Message + "\r\n";
                        recv_box.ScrollToEnd();
                        led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
                    }));
                    so.workSocket.Disconnect(true);
                    so.client.BeginConnect(remote_host, remote_port, new AsyncCallback(ConnectCallback), so);
                }
                return;
            }

            if (read == 0)
            {
                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    recv_box.ScrollToEnd();
                    led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
                }));
                so.workSocket.Disconnect(true);
                so.client.BeginConnect(remote_host, remote_port, new AsyncCallback(ConnectCallback), so);
            }

            if (read > 0)
            {
                so.buffer[read - 1] = (byte)'\r';
                so.buffer[read] = (byte)'\n';
                so.buffer[read + 1] = (byte)'\0';
                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    recv_box.Text += Encoding.Default.GetString(so.buffer, 0, read + 1);
                    recv_box.ScrollToEnd();
                }));
                s.BeginReceive(so.buffer, 0, StateObject.BUFFER_SIZE, 0,
                                         new AsyncCallback(ReceiveCallback), so);
            }
        }

        public void TcpClientSend(string str)
        {
            if (server != null)
            {
                if (server.Connected)
                {
                    server.Client.Send(Encoding.Default.GetBytes(str + "\0"), Encoding.Default.GetByteCount(str + "\0"), 0);
                }
            }
        }
    }
}