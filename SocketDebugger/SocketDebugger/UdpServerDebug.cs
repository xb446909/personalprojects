using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace SocketDebugger
{
    internal class UdpServerDebug
    {
        private Paragraph p = new Paragraph();
        public TextBox connect_state;
        public RichTextBox recv_box;
        public TextBox send_box;
        public Ellipse led;
        private Socket socket;
        private StateObject so = new StateObject();
        private EndPoint remote_ep = new IPEndPoint(IPAddress.Any, 0);

        public UdpServerDebug(object obj)
        {
            Grid main_grid = obj as Grid;
            connect_state = main_grid.FindName("UdpServerConnectStatus") as TextBox;
            recv_box = main_grid.FindName("UdpServerReceiveControl") as RichTextBox;
            send_box = main_grid.FindName("UdpServerSendControl") as TextBox;
            led = main_grid.FindName("UdpServerStateLed") as Ellipse;
        }

        public void UdpServerBind(int port)
        {
            led.Fill = new SolidColorBrush(Color.FromRgb(0, 200, 0));
            connect_state.Text = "Waiting...";
            socket = new Socket(SocketType.Dgram, ProtocolType.Udp);
            EndPoint ep = new IPEndPoint(IPAddress.Any, port);
            socket.Bind(ep);
            so.workSocket = socket;
            so.thread = Thread.CurrentThread;
            try
            {
                so.workSocket.BeginReceiveFrom(so.buffer, 0, StateObject.BUFFER_SIZE, 0, 
                    ref remote_ep, new AsyncCallback(ReceiveCallBack), so);
            }
            catch (Exception e)
            {
                Run run = new Run(e.Message + "\r\n");
                p.Inlines.Add(run);
                recv_box.Document.Blocks.Add(p);
                recv_box.ScrollToEnd();
            }
        }

        public void UdpServerClose()
        {
            led.Fill = new SolidColorBrush(Color.FromRgb(200, 0, 0));
            connect_state.Text = "Closed";
            try
            {
                so.workSocket.Close();
                so.workSocket.Dispose();
            }
            catch (Exception e)
            {
                Run run = new Run(e.Message + "\r\n");
                p.Inlines.Add(run);
                recv_box.Document.Blocks.Add(p);
                recv_box.ScrollToEnd();
            }
        }

        private void ReceiveCallBack(IAsyncResult ar)
        {
            StateObject so = (StateObject)ar.AsyncState;
            so.ar = ar;
            int read = 0;
            
            try
            {
                read = so.workSocket.EndReceiveFrom(ar, ref remote_ep);
            }
            catch (Exception e)
            {
                if (e.HResult == -2146232798)
                {
                    return;
                }
                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    recv_box.AppendText(e.Message + "\r\n");
                    recv_box.ScrollToEnd();
                }));
                so.workSocket.BeginReceiveFrom(so.buffer, 0, StateObject.BUFFER_SIZE, 0,
                    ref remote_ep, new AsyncCallback(ReceiveCallBack), so);
                return;
            }

            if (read > 0)
            {
                Dispatcher.FromThread(so.thread).Invoke(new Action(() =>
                {
                    connect_state.Text = ((IPEndPoint)remote_ep).Address.MapToIPv4().ToString() + ":"
                        + ((IPEndPoint)remote_ep).Port.ToString();

                    Hyperlink hl = new Hyperlink();
                    hl.Foreground = new SolidColorBrush(Colors.Blue);
                    hl.Cursor = Cursors.Hand;
                    hl.Inlines.Add(connect_state.Text);
                    hl.MouseDown += Hl_MouseDown;
                    p.Inlines.Add(hl);
                    Run run = new Run(" :" + Encoding.Default.GetString(so.buffer, 0, read) + "\r\n");
                    p.Inlines.Add(run);
                    recv_box.Document.Blocks.Add(p);
                    recv_box.ScrollToEnd();
                    
                }));
                so.workSocket.BeginReceiveFrom(so.buffer, 0, StateObject.BUFFER_SIZE, 0,
                    ref remote_ep, new AsyncCallback(ReceiveCallBack), so);
            }
        }

        private void Hl_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Hyperlink hl = sender as Hyperlink;
            connect_state.Text = ((Run)hl.Inlines.FirstInline).Text;
            char[] delimiter = { ':' };
            string[] words = connect_state.Text.Split(delimiter);
            ((IPEndPoint)remote_ep).Address = IPAddress.Parse(words[0]);
            ((IPEndPoint)remote_ep).Port = int.Parse(words[1]);
        }

        public void UdpServerSend(string str)
        {
            if (so.workSocket != null)
            {
                try
                {
                    so.workSocket.SendTo(Encoding.Default.GetBytes(str + "\0"),
                        Encoding.Default.GetByteCount(str + "\0"), 0, remote_ep);
                }
                catch (Exception e)
                {
                    Run run = new Run(e.Message + "\r\n");
                    p.Inlines.Add(run);
                    recv_box.Document.Blocks.Add(p);
                    recv_box.ScrollToEnd();
                }
            }
        }
    }
}