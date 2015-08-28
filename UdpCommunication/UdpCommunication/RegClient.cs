using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Timers;

namespace UdpCommunication
{
    internal class RegClient
    {
        Timer timer;
        public Socket socket;
        public IPEndPoint endpoint;
        public string name;

        public RegClient(int interval_s)
        {
            timer = new Timer(interval_s);
            timer.Elapsed += Timer_Elapsed;
        }

        private void Timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            socket.SendTo(Encoding.Default.GetBytes("#REG" + name), endpoint);
        }

        void StartFlap()
        {
            timer.Start();
        }
    }
}