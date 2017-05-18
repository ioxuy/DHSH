namespace UserConsole.AccountForm
{
    partial class AccountInfoForm
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
            this.txtAccountInfo = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // txtAccountInfo
            // 
            this.txtAccountInfo.Location = new System.Drawing.Point(12, 12);
            this.txtAccountInfo.Multiline = true;
            this.txtAccountInfo.Name = "txtAccountInfo";
            this.txtAccountInfo.Size = new System.Drawing.Size(523, 216);
            this.txtAccountInfo.TabIndex = 0;
            // 
            // AccountInfoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(547, 240);
            this.Controls.Add(this.txtAccountInfo);
            this.Name = "AccountInfoForm";
            this.Text = "AccountInfoForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.AccountInfoForm_FormClosing);
            this.Load += new System.EventHandler(this.AccountInfoForm_Load);
            this.SizeChanged += new System.EventHandler(this.AccountInfoForm_SizeChanged);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtAccountInfo;
    }
}