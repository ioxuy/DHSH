using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UserConsole.AccountForm
{
    public partial class AccountInfoForm : Form
    {
        public AccountInfoForm()
        {
            InitializeComponent();
        }
        
        private void AccountInfoForm_Load(object sender, EventArgs e)
        {
            txtAccountInfo.Text = "查询中……";

            var LogContent = new StringBuilder(1024);
            Code.API.QueryCardLog(LogContent);
            txtAccountInfo.Text = LogContent.ToString();
        }

        private void AccountInfoForm_SizeChanged(object sender, EventArgs e)
        {
            txtAccountInfo.Width = this.Width - 50;
            txtAccountInfo.Height = this.Height - 60;
        }

        private void AccountInfoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
        }
    }
}
