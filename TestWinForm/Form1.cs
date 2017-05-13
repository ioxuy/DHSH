using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestWinForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var strArray = textBox1.Text.Trim().Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            Regex reg = new Regex("\"\\S+\"|\\d+");

            var strText = "";
            for (int i = 0; i < strArray.Length; i++)
            {
                var strTempText = strArray[i];
                var v = reg.Matches(strTempText);
                if (v.Count != 5)
                    continue;

                strText += "{ L" + v[0].Value.ToString() + ", L" + v[1].Value.ToString() + ", L" + v[2].Value.ToString() + ", Point(" + v[3].Value.ToString() + ", " + v[4].Value.ToString() + ") },\r\n";
            }
            textBox1.Text = strText;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var strArray = textBox1.Text.Trim().Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            Regex reg = new Regex("\"\\S+\"|\\d+");

            var strText = "";
            for (int i = 0; i < strArray.Length; i++)
            {
                var strTempText = strArray[i];
                var v = reg.Matches(strTempText);
                if (v.Count != 3)
                    continue;

                strText += "{ L" + v[0].Value.ToString() + ", " + v[1].Value.ToString() + ", " + v[2].Value.ToString() + " },\r\n";
            }
            textBox1.Text = strText;
        }
    }
}
