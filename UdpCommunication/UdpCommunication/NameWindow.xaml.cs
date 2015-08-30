using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace UdpCommunication
{
    /// <summary>
    /// NameWindow.xaml 的交互逻辑
    /// </summary>
    public partial class NameWindow : Window
    {
        public string name;
        public NameWindow()
        {
            InitializeComponent();
        }

        private void button_ok_Click(object sender, RoutedEventArgs e)
        {
            name = text_name.Text;
            DialogResult = true;
            Close();
        }
    }
}
