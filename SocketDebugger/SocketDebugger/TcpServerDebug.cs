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
        public Ellipse led;
        
        public TcpServerDebug(object grid)
        {
            Grid main_grid = grid as Grid;
            connect_state = main_grid.FindName("ConnectStatus") as TextBox;
            led = main_grid.FindName("StateLed") as Ellipse;
        }

        public void TcpServerBind(int port)
        {
            led.Fill = new SolidColorBrush(Color.FromRgb(200, 200, 0));
            connect_state.Text = "Waiting...";
            IPAddress Ip = IPAddress.Parse("127.0.0.1");
            TcpListener listener = new TcpListener(Ip, port);
            listener.Start();

            StateObject so = new StateObject();
            so.thread = Thread.CurrentThread;
            so.listener = listener;

            listener.BeginAcceptTcpClient(
                new AsyncCallback(TcpListen_Callback),
                so);

        }

        private void TcpListen_Callback(IAsyncResult ar)
        {
            StateObject so = (StateObject)ar.AsyncState;
            TcpListener listener = so.listener;
            TcpClient client = listener.EndAcceptTcpClient(ar);

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

            int read = s.EndReceive(ar);

            MessageBox.Show(string.Format("Read {0} bytes", read));

            if (read > 0)
            {
                so.sb.Append(Encoding.ASCII.GetString(so.buffer, 0, read));
                MessageBox.Show(string.Format("Read {0} byte from socket" +
                                       "data = {1} ", so.sb.ToString().Length, so.sb.ToString()));
                s.BeginReceive(so.buffer, 0, StateObject.BUFFER_SIZE, 0,
                                         new AsyncCallback(TcpReceive_CallBack), so);
            }
            /*            else
                        {
                            if (so.sb.Length > 1)
                            {
                                //All of the data has been read, so displays it to the console 
                                string strContent;
                                strContent = so.sb.ToString();
                                MessageBox.Show(string.Format("Read {0} byte from socket" +
                                                   "data = {1} ", strContent.Length, strContent));
                            }
                            s.Close();
                        }*/
        }
    }
}