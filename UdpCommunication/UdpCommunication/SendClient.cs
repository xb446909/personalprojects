using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Timers;
using System.Windows;

namespace UdpCommunication
{
    internal class SendClient
    {
        Timer timer;
        Socket socket;
        IPEndPoint endpoint;

        public SendClient(int interval)
        {
            timer = new Timer(interval * 1000);
            timer.Elapsed += Timer_Elapsed;
        }

        private void Timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            socket.SendTo(Encoding.Default.GetBytes("#MSG#"), endpoint);
            //MessageBox.Show("Send MSG to " + endpoint.Address.ToString() + ":" + endpoint.Port.ToString());
        }

        public void StartSend(Socket so, IPEndPoint ep)
        {
            if (timer.Enabled == true)
            {
                timer.Enabled = false;
            }
            socket = so;
            endpoint = ep;
            Timer_Elapsed(null, null);
            timer.Start();
        }
    }
}