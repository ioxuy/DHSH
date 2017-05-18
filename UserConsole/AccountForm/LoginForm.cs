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
    public partial class LoginForm : Form
    {
        public LoginForm()
        {
            InitializeComponent();
        }

        private bool _bLogin = false;

        private void btnLogin_Click(object sender, EventArgs e)
        {
            if (txtAccount.Text.Trim().Length == 0 || txtPass.Text.Trim().Length == 0)
            {
                MessageBox.Show("帐号密码非空!");
                return;
            }

            int nRetCode = Code.API.Login(txtAccount.Text.Trim(), txtPass.Text.Trim());
            if (nRetCode == 0)
            {
                _bLogin = true;
                this.Close();
            }
            else if (nRetCode == 1)
                MessageBox.Show("帐号密码不正确!");
            else if (nRetCode == 2)
                MessageBox.Show("网络有异常……重新登录试试~");
            else
                MessageBox.Show("RetCode=" + nRetCode.ToString());
        }

        private void btnRegister_Click(object sender, EventArgs e)
        {
            if (txtAccount.Text.Trim().Length == 0 || txtPass.Text.Trim().Length == 0)
            {
                MessageBox.Show("帐号密码非空!");
                return;
            }

            int nRetCode = Code.API.Register(txtAccount.Text.Trim(), txtPass.Text.Trim());
            if (nRetCode == 0)
            {
                MessageBox.Show("注册成功!!");
                btnLogin_Click(sender, e);
                return;
            }
            else if (nRetCode == 1)
                MessageBox.Show("该帐号已经被用了……");
            else if (nRetCode == 2)
                MessageBox.Show("网络有异常……重新登录试试~");
            else
                MessageBox.Show("RetCode=" + nRetCode.ToString());
        }
    }
}
