using System;
using System.Net;
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
    internal class TcpServerDebug
    {
        public TextBox connect_state;
        public TextBox recv_box;
        public TextBox send_box;
        public Ellipse led;
        private TcpClient client;
        private StateObject so = new StateObject();

        public TcpServerDebug(object grid)
        {
            Grid main_grid = grid as Grid;
            connect_state = main_grid.FindName("ConnectStatus") as TextBox;
            recv_box = main_grid.FindName("ReceiveControl") as TextBox;
            send_box = main_grid.FindName("SendControl") as TextBox;
            led = main_grid.FindName("StateLed") as Ellipse;
        }

        public void TcpServerBind(int port)
        {
            led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
            connect_state.Text = "Waiting...";
            IPAddress Ip = IPAddress.Parse("127.0.0.1");
            TcpListener listener = new TcpListener(Ip, port);
            listener.Start();

            so.thread = Thread.CurrentThread;
            so.listener = listener;

            listener.BeginAcceptTcpClient(
                new AsyncCallback(TcpListen_Callback),
                so);

        }

        public void TcpServerStopConnect()
        {
            if (so.listener != null)
            {
                try
                {
                    so.listener.Stop();
                    so.workSocket.Disconnect(false);
                    so.workSocket.Close();
                    so.listener = null;
                }
                catch (Exception e)
                {
                    recv_box.Text += e.Message + "\r\n";
                }
            }
            led.Fill = new SolidColorBrush(Color.FromRgb(200, 0, 0));
            connect_state.Text = "Closed";
        }

        private void TcpListen_Callback(IAsyncResult ar)
        {
            StateObject so = (StateObject)ar.AsyncState;
            TcpListener listener = so.listener;

            try
            {
                client = listener.EndAcceptTcpClient(ar);
            }
            catch (Exception e)
            {
                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    recv_box.Text += e.Message + "\r\n";
                }));
                    return;
            }

            Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
            {
                led.Fill = new SolidColorBrush(Color.FromRgb(0, 200, 0));
                connect_state.Text = ((IPEndPoint)client.Client.RemoteEndPoint).Address.ToString() + ":" +
                    ((IPEndPoint)client.Client.RemoteEndPoint).Port.ToString();
            }));

            so.workSocket = client.Client;

            so.workSocket.BeginReceive(so.buffer, 0, StateObject.BUFFER_SIZE, 0,
                new AsyncCallback(TcpReceive_CallBack), so);
        }

        private void TcpReceive_CallBack(IAsyncResult ar)
        {
            StateObject so = (StateObject)ar.AsyncState;
            Socket s = so.workSocket;

            int read = 0;

            try
            {
                read = s.EndReceive(ar);
            }
            catch(Exception e)
            {
                if (so.listener != null)
                {
                    so.listener.BeginAcceptTcpClient(
                        new AsyncCallback(TcpListen_Callback),
                        so);

                    Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                    {
                        recv_box.Text += e.Message + "\r\n";
                        recv_box.ScrollToEnd();
                        connect_state.Text = "Waiting...";
                        led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
                    }));
                }

                return;
            }

            if (read == 0)
            {
                so.listener.BeginAcceptTcpClient(
                    new AsyncCallback(TcpListen_Callback),
                    so);

                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    connect_state.Text = "Waiting...";
                    led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
                }));
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
                                         new AsyncCallback(TcpReceive_CallBack), so);
            }
        }

        public void TcpServerSend(string str)
        {
            if(client != null)
            {
                if(client.Connected)
                {
                    client.Client.Send(Encoding.Default.GetBytes(str + "\0"), Encoding.Default.GetByteCount(str + "\0"), 0);
                }
            }
        }
    }
}