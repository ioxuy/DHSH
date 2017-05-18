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
    public partial class RechargeCardForm : Form
    {
        public RechargeCardForm()
        {
            InitializeComponent();
        }

        private void RechargeCardForm_Load(object sender, EventArgs e)
        {

        }

        private void btnSumit_Click(object sender, EventArgs e)
        {
            if (Code.API.CheckCard(txtCard.Text.Trim()) == 0)
            {
                MessageBox.Show("格式错误!");
                return;
            }

            var VarText = new StringBuilder(1024);
            Code.API.RechargeCard(txtCard.Text.Trim(), VarText);
            MessageBox.Show(VarText.ToString());
            this.Close();
        }
    }
}
