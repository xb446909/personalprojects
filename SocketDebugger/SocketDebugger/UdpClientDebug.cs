using System.Windows.Controls;
using System.Windows.Shapes;

namespace SocketDebugger
{
    internal class UdpClientDebug
    {
        public TextBox recv_box;
        public TextBox send_box;
        public UdpClientDebug(object obj)
        {
            Grid main_grid = obj as Grid;
            recv_box = main_grid.FindName("UdpClientReceiveControl") as TextBox;
            send_box = main_grid.FindName("UdpClientSendControl") as TextBox;
        }

        public void UdpClientSend(string ip_addr, int port, string msg)
        {

        }
    }
}