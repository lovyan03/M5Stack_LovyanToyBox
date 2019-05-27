namespace ScreenShotSender
{
    partial class FormSenderMain
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnShowFrame = new System.Windows.Forms.Button();
            this.btnStartStopr = new System.Windows.Forms.Button();
            this.pnlTop1 = new System.Windows.Forms.Panel();
            this.nudPacketSize = new System.Windows.Forms.NumericUpDown();
            this.lblPakcetSize = new System.Windows.Forms.Label();
            this.tbHost = new System.Windows.Forms.TextBox();
            this.nudQuality = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.nudHeight = new System.Windows.Forms.NumericUpDown();
            this.nudWidth = new System.Windows.Forms.NumericUpDown();
            this.pbPreview = new System.Windows.Forms.PictureBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.pnlAll = new System.Windows.Forms.Panel();
            this.pnlTop2 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.nudDelay = new System.Windows.Forms.NumericUpDown();
            this.lblDelay = new System.Windows.Forms.Label();
            this.nudDivide = new System.Windows.Forms.NumericUpDown();
            this.lblDivide = new System.Windows.Forms.Label();
            this.lblHeight = new System.Windows.Forms.Label();
            this.lblWidth = new System.Windows.Forms.Label();
            this.pnlTop1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPacketSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudQuality)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbPreview)).BeginInit();
            this.pnlAll.SuspendLayout();
            this.pnlTop2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelay)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDivide)).BeginInit();
            this.SuspendLayout();
            // 
            // btnShowFrame
            // 
            this.btnShowFrame.Dock = System.Windows.Forms.DockStyle.Left;
            this.btnShowFrame.Location = new System.Drawing.Point(0, 0);
            this.btnShowFrame.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.btnShowFrame.Name = "btnShowFrame";
            this.btnShowFrame.Size = new System.Drawing.Size(132, 38);
            this.btnShowFrame.TabIndex = 0;
            this.btnShowFrame.Text = "ShowBox";
            this.btnShowFrame.UseVisualStyleBackColor = true;
            this.btnShowFrame.Click += new System.EventHandler(this.btnShowFrame_Click);
            // 
            // btnStartStopr
            // 
            this.btnStartStopr.Dock = System.Windows.Forms.DockStyle.Left;
            this.btnStartStopr.Location = new System.Drawing.Point(132, 0);
            this.btnStartStopr.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.btnStartStopr.Name = "btnStartStopr";
            this.btnStartStopr.Size = new System.Drawing.Size(132, 38);
            this.btnStartStopr.TabIndex = 1;
            this.btnStartStopr.Text = "Start";
            this.btnStartStopr.UseVisualStyleBackColor = true;
            this.btnStartStopr.Click += new System.EventHandler(this.btnStartStop_Click);
            // 
            // pnlTop1
            // 
            this.pnlTop1.Controls.Add(this.nudPacketSize);
            this.pnlTop1.Controls.Add(this.lblPakcetSize);
            this.pnlTop1.Controls.Add(this.tbHost);
            this.pnlTop1.Controls.Add(this.btnStartStopr);
            this.pnlTop1.Controls.Add(this.btnShowFrame);
            this.pnlTop1.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop1.Location = new System.Drawing.Point(0, 0);
            this.pnlTop1.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.pnlTop1.Name = "pnlTop1";
            this.pnlTop1.Size = new System.Drawing.Size(724, 38);
            this.pnlTop1.TabIndex = 2;
            // 
            // nudPacketSize
            // 
            this.nudPacketSize.Dock = System.Windows.Forms.DockStyle.Left;
            this.nudPacketSize.Location = new System.Drawing.Point(547, 0);
            this.nudPacketSize.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.nudPacketSize.Maximum = new decimal(new int[] {
            1460,
            0,
            0,
            0});
            this.nudPacketSize.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nudPacketSize.Name = "nudPacketSize";
            this.nudPacketSize.Size = new System.Drawing.Size(93, 31);
            this.nudPacketSize.TabIndex = 12;
            this.nudPacketSize.Value = new decimal(new int[] {
            1460,
            0,
            0,
            0});
            this.nudPacketSize.Visible = false;
            // 
            // lblPakcetSize
            // 
            this.lblPakcetSize.AutoSize = true;
            this.lblPakcetSize.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblPakcetSize.Location = new System.Drawing.Point(426, 0);
            this.lblPakcetSize.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.lblPakcetSize.MinimumSize = new System.Drawing.Size(0, 38);
            this.lblPakcetSize.Name = "lblPakcetSize";
            this.lblPakcetSize.Size = new System.Drawing.Size(121, 38);
            this.lblPakcetSize.TabIndex = 11;
            this.lblPakcetSize.Text = "PakcetSize";
            this.lblPakcetSize.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.lblPakcetSize.Visible = false;
            // 
            // tbHost
            // 
            this.tbHost.Dock = System.Windows.Forms.DockStyle.Left;
            this.tbHost.Location = new System.Drawing.Point(264, 0);
            this.tbHost.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.tbHost.Name = "tbHost";
            this.tbHost.Size = new System.Drawing.Size(162, 31);
            this.tbHost.TabIndex = 2;
            this.tbHost.Text = "192.168.1.1";
            // 
            // nudQuality
            // 
            this.nudQuality.Dock = System.Windows.Forms.DockStyle.Left;
            this.nudQuality.Location = new System.Drawing.Point(398, 0);
            this.nudQuality.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.nudQuality.Name = "nudQuality";
            this.nudQuality.Size = new System.Drawing.Size(93, 31);
            this.nudQuality.TabIndex = 14;
            this.nudQuality.Value = new decimal(new int[] {
            60,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Left;
            this.label2.Location = new System.Drawing.Point(319, 0);
            this.label2.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.label2.MinimumSize = new System.Drawing.Size(0, 38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 38);
            this.label2.TabIndex = 13;
            this.label2.Text = "Quality";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // nudHeight
            // 
            this.nudHeight.Dock = System.Windows.Forms.DockStyle.Left;
            this.nudHeight.Location = new System.Drawing.Point(226, 0);
            this.nudHeight.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.nudHeight.Maximum = new decimal(new int[] {
            240,
            0,
            0,
            0});
            this.nudHeight.Minimum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.nudHeight.Name = "nudHeight";
            this.nudHeight.Size = new System.Drawing.Size(93, 31);
            this.nudHeight.TabIndex = 4;
            this.nudHeight.Value = new decimal(new int[] {
            120,
            0,
            0,
            0});
            // 
            // nudWidth
            // 
            this.nudWidth.Dock = System.Windows.Forms.DockStyle.Left;
            this.nudWidth.Location = new System.Drawing.Point(63, 0);
            this.nudWidth.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.nudWidth.Maximum = new decimal(new int[] {
            320,
            0,
            0,
            0});
            this.nudWidth.Minimum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.nudWidth.Name = "nudWidth";
            this.nudWidth.Size = new System.Drawing.Size(93, 31);
            this.nudWidth.TabIndex = 3;
            this.nudWidth.Value = new decimal(new int[] {
            160,
            0,
            0,
            0});
            // 
            // pbPreview
            // 
            this.pbPreview.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pbPreview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pbPreview.Location = new System.Drawing.Point(0, 0);
            this.pbPreview.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.pbPreview.Name = "pbPreview";
            this.pbPreview.Size = new System.Drawing.Size(724, 484);
            this.pbPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pbPreview.TabIndex = 3;
            this.pbPreview.TabStop = false;
            // 
            // timer1
            // 
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // pnlAll
            // 
            this.pnlAll.Controls.Add(this.pbPreview);
            this.pnlAll.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlAll.Location = new System.Drawing.Point(0, 72);
            this.pnlAll.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.pnlAll.Name = "pnlAll";
            this.pnlAll.Size = new System.Drawing.Size(724, 484);
            this.pnlAll.TabIndex = 4;
            // 
            // pnlTop2
            // 
            this.pnlTop2.Controls.Add(this.label1);
            this.pnlTop2.Controls.Add(this.nudDelay);
            this.pnlTop2.Controls.Add(this.lblDelay);
            this.pnlTop2.Controls.Add(this.nudDivide);
            this.pnlTop2.Controls.Add(this.lblDivide);
            this.pnlTop2.Controls.Add(this.nudQuality);
            this.pnlTop2.Controls.Add(this.label2);
            this.pnlTop2.Controls.Add(this.nudHeight);
            this.pnlTop2.Controls.Add(this.lblHeight);
            this.pnlTop2.Controls.Add(this.nudWidth);
            this.pnlTop2.Controls.Add(this.lblWidth);
            this.pnlTop2.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop2.Location = new System.Drawing.Point(0, 38);
            this.pnlTop2.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.pnlTop2.Name = "pnlTop2";
            this.pnlTop2.Size = new System.Drawing.Size(724, 34);
            this.pnlTop2.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Left;
            this.label1.ImageAlign = System.Drawing.ContentAlignment.TopLeft;
            this.label1.Location = new System.Drawing.Point(805, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.label1.MinimumSize = new System.Drawing.Size(0, 38);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 38);
            this.label1.TabIndex = 11;
            this.label1.Text = "msec";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.label1.Visible = false;
            // 
            // nudDelay
            // 
            this.nudDelay.Dock = System.Windows.Forms.DockStyle.Left;
            this.nudDelay.Location = new System.Drawing.Point(712, 0);
            this.nudDelay.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.nudDelay.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudDelay.Name = "nudDelay";
            this.nudDelay.Size = new System.Drawing.Size(93, 31);
            this.nudDelay.TabIndex = 8;
            this.nudDelay.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nudDelay.Visible = false;
            // 
            // lblDelay
            // 
            this.lblDelay.AutoSize = true;
            this.lblDelay.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblDelay.Location = new System.Drawing.Point(651, 0);
            this.lblDelay.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.lblDelay.MinimumSize = new System.Drawing.Size(0, 38);
            this.lblDelay.Name = "lblDelay";
            this.lblDelay.Size = new System.Drawing.Size(61, 38);
            this.lblDelay.TabIndex = 10;
            this.lblDelay.Text = "delay";
            this.lblDelay.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.lblDelay.Visible = false;
            // 
            // nudDivide
            // 
            this.nudDivide.Dock = System.Windows.Forms.DockStyle.Left;
            this.nudDivide.Location = new System.Drawing.Point(558, 0);
            this.nudDivide.Margin = new System.Windows.Forms.Padding(2, 4, 2, 4);
            this.nudDivide.Maximum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.nudDivide.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudDivide.Name = "nudDivide";
            this.nudDivide.Size = new System.Drawing.Size(93, 31);
            this.nudDivide.TabIndex = 7;
            this.nudDivide.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.nudDivide.Visible = false;
            // 
            // lblDivide
            // 
            this.lblDivide.AutoSize = true;
            this.lblDivide.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblDivide.Location = new System.Drawing.Point(491, 0);
            this.lblDivide.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.lblDivide.MinimumSize = new System.Drawing.Size(0, 38);
            this.lblDivide.Name = "lblDivide";
            this.lblDivide.Size = new System.Drawing.Size(67, 38);
            this.lblDivide.TabIndex = 9;
            this.lblDivide.Text = "divide";
            this.lblDivide.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.lblDivide.Visible = false;
            // 
            // lblHeight
            // 
            this.lblHeight.AutoSize = true;
            this.lblHeight.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblHeight.Location = new System.Drawing.Point(156, 0);
            this.lblHeight.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.lblHeight.MinimumSize = new System.Drawing.Size(0, 38);
            this.lblHeight.Name = "lblHeight";
            this.lblHeight.Size = new System.Drawing.Size(70, 38);
            this.lblHeight.TabIndex = 6;
            this.lblHeight.Text = "height";
            this.lblHeight.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblWidth
            // 
            this.lblWidth.AutoSize = true;
            this.lblWidth.Dock = System.Windows.Forms.DockStyle.Left;
            this.lblWidth.Location = new System.Drawing.Point(0, 0);
            this.lblWidth.Margin = new System.Windows.Forms.Padding(7, 0, 7, 0);
            this.lblWidth.MinimumSize = new System.Drawing.Size(0, 38);
            this.lblWidth.Name = "lblWidth";
            this.lblWidth.Size = new System.Drawing.Size(63, 38);
            this.lblWidth.TabIndex = 5;
            this.lblWidth.Text = "width";
            this.lblWidth.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // FormSenderMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(13F, 24F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(724, 556);
            this.Controls.Add(this.pnlAll);
            this.Controls.Add(this.pnlTop2);
            this.Controls.Add(this.pnlTop1);
            this.Margin = new System.Windows.Forms.Padding(7, 6, 7, 6);
            this.Name = "FormSenderMain";
            this.Text = "ScreenShotSender";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormSenderMain_FormClosed);
            this.Shown += new System.EventHandler(this.FormSenderMain_Shown);
            this.pnlTop1.ResumeLayout(false);
            this.pnlTop1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPacketSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudQuality)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbPreview)).EndInit();
            this.pnlAll.ResumeLayout(false);
            this.pnlTop2.ResumeLayout(false);
            this.pnlTop2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudDelay)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudDivide)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnShowFrame;
        private System.Windows.Forms.Button btnStartStopr;
        private System.Windows.Forms.Panel pnlTop1;
        private System.Windows.Forms.PictureBox pbPreview;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox tbHost;
        private System.Windows.Forms.Panel pnlAll;
        private System.Windows.Forms.NumericUpDown nudWidth;
        private System.Windows.Forms.NumericUpDown nudHeight;
        private System.Windows.Forms.Panel pnlTop2;
        private System.Windows.Forms.Label lblHeight;
        private System.Windows.Forms.Label lblWidth;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nudDelay;
        private System.Windows.Forms.Label lblDelay;
        private System.Windows.Forms.NumericUpDown nudDivide;
        private System.Windows.Forms.Label lblDivide;
        private System.Windows.Forms.NumericUpDown nudPacketSize;
        private System.Windows.Forms.Label lblPakcetSize;
        private System.Windows.Forms.NumericUpDown nudQuality;
        private System.Windows.Forms.Label label2;
    }
}

