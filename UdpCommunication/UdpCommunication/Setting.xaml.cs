using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace UdpCommunication
{
    /// <summary>
    /// Interaction logic for Setting.xaml
    /// </summary>
    public partial class Setting : Window
    {
        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        public static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);

        Brush default_brush = new SolidColorBrush(Color.FromRgb(255, 255, 255));

        public Setting()
        {
            InitializeComponent();
            Background = default_brush;
        }

        private void OnCloseWindow(object sender, MouseButtonEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        private void OnMouseEnter(object sender, MouseEventArgs e)
        {
            Canvas can = sender as Canvas;
            can.Background = Brushes.Gray;
        }

        private void OnMouseLeave(object sender, MouseEventArgs e)
        {
            Canvas can = sender as Canvas;
            can.Background = default_brush;
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

        private void button_save_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void button_cancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }
    }
}
