using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Timers;
using System.Windows;
using System.Windows.Controls;

namespace UdpCommunication
{
    internal class RegClient
    {
        System.Timers.Timer timer;
        System.Timers.Timer update_timer;
        Socket socket;
        IPEndPoint endpoint;
        public string name;

        public RegClient(int interval_s, object obj)
        {
            timer = new System.Timers.Timer(interval_s * 1000);
            timer.Elapsed += Timer_Elapsed;

            update_timer = new System.Timers.Timer(2 * interval_s * 1000);
            update_timer.Elapsed += Update_timer_Elapsed;
        }

        private void Update_timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            socket.SendTo(Encoding.Default.GetBytes("#LST"), endpoint);
        }

        private void Timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            socket.SendTo(Encoding.Default.GetBytes("#REG" + name), endpoint);
        }

        public void StartFlap(Socket so, IPEndPoint ep)
        {
            socket = so;
            endpoint = ep;
            timer.Start();
            update_timer.Start();
            Timer_Elapsed(null, null);
            Update_timer_Elapsed(null, null);
        }

    }
}