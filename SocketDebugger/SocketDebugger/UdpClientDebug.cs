using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Threading;

namespace SocketDebugger
{
    internal class UdpClientDebug
    {
        public TextBox recv_box;
        public TextBox send_box;
        StateObject so = new StateObject();

        public UdpClientDebug(object obj)
        {
            Grid main_grid = obj as Grid;
            recv_box = main_grid.FindName("UdpClientReceiveControl") as TextBox;
            send_box = main_grid.FindName("UdpClientSendControl") as TextBox;

            so.workSocket = new Socket(SocketType.Dgram, ProtocolType.Udp);
            so.thread = Thread.CurrentThread;
        }

        private void UdpClientBeginRecv()
        {
            EndPoint ep = new IPEndPoint(IPAddress.Any, 0);
            try
            {
                so.ar = so.workSocket.BeginReceiveFrom(so.buffer, 0, StateObject.BUFFER_SIZE, 0, ref ep, new AsyncCallback(ReceiveCallback), so);
            }
            catch (Exception e)
            {
                recv_box.Text += e.Message + "\r\n";
            }
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            StateObject so2 = ar.AsyncState as StateObject;
            int read = 0;
            EndPoint ep = new IPEndPoint(IPAddress.Any, 0);

            try
            {
                read = so2.workSocket.EndReceiveFrom(ar, ref ep);
            }
            catch (Exception e)
            {
                Dispatcher.FromThread(so2.thread).Invoke(new Action(() =>
                {
                    recv_box.Text += e.Message + "\r\n";
                }));
            }

            if (read > 0)
            {
                Dispatcher.FromThread(so2.thread).Invoke(new Action(() =>
                {
                    recv_box.Text += Encoding.Default.GetString(so2.buffer, 0, read - 1) + "\r\n";
                }));
                so2.ar = so2.workSocket.BeginReceiveFrom(so2.buffer, 0, StateObject.BUFFER_SIZE, 0, 
                    ref ep, new AsyncCallback(ReceiveCallback), so2);
            }
        }

        public void UdpClientSend(string ip_addr, int port, string msg)
        {
            int index = -1;
            var address = Dns.GetHostEntry(ip_addr);
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
                recv_box.Text += "无法从" + ip_addr + "获得IP地址.\r\n";
                return;
            }

            EndPoint sendep = new IPEndPoint(address.AddressList[index], port);
            so.workSocket.SendTo(Encoding.Default.GetBytes(msg + "\r\n"),
                Encoding.Default.GetByteCount(msg + "\r\n"), 0, sendep);

            UdpClientBeginRecv();
        }
    }
}