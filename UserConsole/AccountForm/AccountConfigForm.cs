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
    public partial class AccountConfigForm : Form
    {
        public AccountConfigForm(String PlayerName)
        {
            InitializeComponent();
            _PlayerName = PlayerName;
        }

        private void AccountConfigForm_Load(object sender, EventArgs e)
        {
            this.Text = "读取云服务的配置文件中……";
            this.tabControl1.Enabled = false;


        }

        private String _PlayerName = string.Empty;
       
    }

    public class CAccountConfigContent
    {
        public AccountConfigForm ConfigForm { get; set; }

        public Code.CAccount Account { get; set; }
    }
}
