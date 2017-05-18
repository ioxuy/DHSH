namespace UserConsole.AccountForm
{
    partial class RechargeCardForm
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
            this.txtCard = new System.Windows.Forms.TextBox();
            this.btnSumit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // txtCard
            // 
            this.txtCard.Font = new System.Drawing.Font("SimSun", 20F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.txtCard.Location = new System.Drawing.Point(23, 12);
            this.txtCard.Name = "txtCard";
            this.txtCard.Size = new System.Drawing.Size(391, 38);
            this.txtCard.TabIndex = 0;
            // 
            // btnSumit
            // 
            this.btnSumit.Location = new System.Drawing.Point(142, 66);
            this.btnSumit.Name = "btnSumit";
            this.btnSumit.Size = new System.Drawing.Size(114, 36);
            this.btnSumit.TabIndex = 1;
            this.btnSumit.Text = "充值";
            this.btnSumit.UseVisualStyleBackColor = true;
            this.btnSumit.Click += new System.EventHandler(this.btnSumit_Click);
            // 
            // RechargeCardForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(426, 114);
            this.Controls.Add(this.btnSumit);
            this.Controls.Add(this.txtCard);
            this.Name = "RechargeCardForm";
            this.Text = "RechargeCardForm";
            this.Load += new System.EventHandler(this.RechargeCardForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtCard;
        private System.Windows.Forms.Button btnSumit;
    }
}