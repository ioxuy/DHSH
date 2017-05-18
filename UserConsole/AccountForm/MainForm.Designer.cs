namespace UserConsole.AccountForm
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.查询帐号ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.卡号ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.开始自动登录ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.手动登录ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label1 = new System.Windows.Forms.Label();
            this.lbHour = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.查询帐号ToolStripMenuItem,
            this.卡号ToolStripMenuItem,
            this.开始自动登录ToolStripMenuItem,
            this.手动登录ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(567, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 查询帐号ToolStripMenuItem
            // 
            this.查询帐号ToolStripMenuItem.Name = "查询帐号ToolStripMenuItem";
            this.查询帐号ToolStripMenuItem.Size = new System.Drawing.Size(71, 20);
            this.查询帐号ToolStripMenuItem.Text = "查询帐号";
            this.查询帐号ToolStripMenuItem.Click += new System.EventHandler(this.查询帐号ToolStripMenuItem_Click);
            // 
            // 卡号ToolStripMenuItem
            // 
            this.卡号ToolStripMenuItem.Name = "卡号ToolStripMenuItem";
            this.卡号ToolStripMenuItem.Size = new System.Drawing.Size(71, 20);
            this.卡号ToolStripMenuItem.Text = "卡号充值";
            this.卡号ToolStripMenuItem.Click += new System.EventHandler(this.卡号ToolStripMenuItem_Click);
            // 
            // 开始自动登录ToolStripMenuItem
            // 
            this.开始自动登录ToolStripMenuItem.Name = "开始自动登录ToolStripMenuItem";
            this.开始自动登录ToolStripMenuItem.Size = new System.Drawing.Size(71, 20);
            this.开始自动登录ToolStripMenuItem.Text = "自动登录";
            // 
            // 手动登录ToolStripMenuItem
            // 
            this.手动登录ToolStripMenuItem.Name = "手动登录ToolStripMenuItem";
            this.手动登录ToolStripMenuItem.Size = new System.Drawing.Size(71, 20);
            this.手动登录ToolStripMenuItem.Text = "启动游戏";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column1,
            this.Column3,
            this.Column2});
            this.dataGridView1.Location = new System.Drawing.Point(11, 25);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(546, 291);
            this.dataGridView1.TabIndex = 1;
            // 
            // Column1
            // 
            this.Column1.HeaderText = "帐号";
            this.Column1.Name = "Column1";
            this.Column1.Width = 150;
            // 
            // Column3
            // 
            this.Column3.HeaderText = "角色";
            this.Column3.Name = "Column3";
            // 
            // Column2
            // 
            this.Column2.HeaderText = "消息";
            this.Column2.Name = "Column2";
            this.Column2.Width = 250;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 320);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "剩余时间/小时:";
            // 
            // lbHour
            // 
            this.lbHour.AutoSize = true;
            this.lbHour.ForeColor = System.Drawing.Color.Red;
            this.lbHour.Location = new System.Drawing.Point(103, 321);
            this.lbHour.Name = "lbHour";
            this.lbHour.Size = new System.Drawing.Size(11, 12);
            this.lbHour.TabIndex = 3;
            this.lbHour.Text = "0";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(567, 336);
            this.Controls.Add(this.lbHour);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.menuStrip1);
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.SizeChanged += new System.EventHandler(this.MainForm_SizeChanged);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 查询帐号ToolStripMenuItem;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lbHour;
        private System.Windows.Forms.ToolStripMenuItem 卡号ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 开始自动登录ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 手动登录ToolStripMenuItem;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column2;
    }
}