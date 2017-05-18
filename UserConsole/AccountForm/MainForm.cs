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

namespace UserConsole.AccountForm
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void 查询帐号ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new AccountForm.AccountInfoForm().ShowDialog();
        }

        private void 卡号ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new AccountForm.RechargeCardForm().ShowDialog();
            lbHour.Text = Code.API.GetHour().ToString();
        }

        void SetGridViewCell_Color(Color color, int i)
        {
            DataGridViewCellStyle DGVCS = new System.Windows.Forms.DataGridViewCellStyle();
            DGVCS.ForeColor = color;

            for (int k = 0; k < dataGridView1.Columns.Count; k++)
            {
                dataGridView1.Rows[i].Cells[k].Style = DGVCS;
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            Code.ConsoleVar.g_AccountList = Code.AccountLogic.GetAccountList();
            lbHour.Text = Code.API.GetHour().ToString();

            foreach (var itm in Code.ConsoleVar.g_AccountList)
            {
                dataGridView1.Rows.Add();

                dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[0].Value = itm.AccountName;
                dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[1].Value = itm.PlayerName;
                dataGridView1.Rows[dataGridView1.Rows.Count - 1].Cells[2].Value = "尚未运行……";
            }
            
        }

        private void MainForm_SizeChanged(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - (583 - 546);
            dataGridView1.Height = this.Height - (375 - 291);
        }

        private bool ExistPlayerName(String PlayerName)
        {
            return Code.ConsoleVar.g_AccountList.Find(itm => itm.PlayerName == PlayerName) != null;
        }

        private List<CAccountConfigContent> _ListAccountConfig = new List<CAccountConfigContent>();
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            // HOME
            if (m.Msg == 0x400 + 0x52)
            {
                var PlayerName = Marshal.PtrToStringAuto(m.LParam);
                if (!ExistPlayerName(PlayerName))
                {
                    MessageBox.Show(String.Format("角色:{0} 不存在帐号列表中,请确认该角色在Account.txt目录下,并且用‘自动登录‘或者‘手动登录’来开启游戏", PlayerName));
                    return;
                }
                var AccountElement = _ListAccountConfig.Find(itm => itm.Account.PlayerName == PlayerName);
                if (AccountElement == null)
                {
                    var NewForm = new CAccountConfigContent() { Account = Code.ConsoleVar.g_AccountList.Find(itm => itm.PlayerName == PlayerName), ConfigForm = new AccountConfigForm(PlayerName) };
                    NewForm.ConfigForm.Show();
                    _ListAccountConfig.Add(NewForm);
                }
                else
                {
                    AccountElement.ConfigForm.Show();
                }
            }
            else if (m.Msg == 0x400 + 0x53) // END
            {
                var PlayerName = Marshal.PtrToStringAuto(m.LParam);
                if (!ExistPlayerName(PlayerName))
                {
                    MessageBox.Show(String.Format("角色:{0} 不存在帐号列表中,请确认该角色在Account.txt目录下,并且用‘自动登录‘或者‘手动登录’来开启游戏", PlayerName));
                    return;
                }

                var AccountElement = _ListAccountConfig.Find(itm => itm.Account.PlayerName == PlayerName);
                if (AccountElement != null)
                {
                    AccountElement.ConfigForm.Hide();
                }
            }
        }
    }
}
