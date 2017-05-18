using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UserConsole
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           
        }

        [DllImport("user32.dll")]
        public static extern int FindWindow(string lpClassName, String lpWindowName);
        [DllImport("user32.dll")]
        public static extern int SendMessage(int hWnd, int wMsg, int wParam, IntPtr lParam);

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            // HOME
            if (m.Msg == 0x400 + 0x52)
            {
                MessageBox.Show("HOME");
                var v = Marshal.PtrToStringAuto(m.LParam);
                MessageBox.Show(v.ToString());

            }
            else if (m.Msg == 0x400 + 0x53) // END
            {
                MessageBox.Show("END");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int hwnd = FindWindow(null,"FFKKSS");
            SendMessage(hwnd, 0x400 + 0x52, 0, Marshal.StringToHGlobalAuto("asdasd"));
        }
    }
}
