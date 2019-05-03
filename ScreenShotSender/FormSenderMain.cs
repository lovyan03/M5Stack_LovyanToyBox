using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ScreenShotSender
{
    public partial class FormSenderMain : Form
    {
        FormCaptureBox formCaptureBox = new FormCaptureBox();
        ImageCodecInfo _jpgEncoder = null;
        Bitmap _resizeBmp;
        Graphics _gResizeBmp;
        Bitmap _jpgBuf;
        Graphics _gJpgBuf;
        Int64 _jpgQuality = 60;
        int _udpPort = 63333;

        ProptotypeControler.UDPSender _udp;
        System.Diagnostics.Stopwatch _sw;
        public FormSenderMain()
        {
            InitializeComponent();
            updateBmp();
            _sw = new System.Diagnostics.Stopwatch();
            _udp = new ProptotypeControler.UDPSender();
            _udp.init(_udpPort-1, _udpPort);

            foreach (ImageCodecInfo ici in ImageCodecInfo.GetImageEncoders()) {
                if (ici.FormatID == ImageFormat.Jpeg.Guid) {
                    _jpgEncoder = ici;
                    break;
                }
            }
        }

        private void FormSenderMain_Shown(object sender, EventArgs e)
        {
            FrameVisible(true);
            formCaptureBox.Location = new Point(Location.X + 10, Location.Y + Height);
            timer1.Enabled = true;
        }

        private void FrameVisible(bool visible)
        {
            formCaptureBox.Visible = visible;
            btnShowFrame.Text = visible ? "HideFrame" : "ShowFrame";
        }
        private void btnShowFrame_Click(object sender, EventArgs e)
        {
            FrameVisible(!formCaptureBox.Visible);
        }

        public void updateBmp()
        {
            if (_resizeBmp == null || _resizeBmp.Width != nudWidth.Value || _resizeBmp.Height != nudHeight.Value)
            {
                _gResizeBmp?.Dispose();
                _resizeBmp?.Dispose();
                _resizeBmp = new Bitmap((int)nudWidth.Value, (int)nudHeight.Value, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                _gResizeBmp = Graphics.FromImage(_resizeBmp);
                _gResizeBmp.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;  // AntiAlias; //  HighQuality;
            }
        }

        private void btnStartStop_Click(object sender, EventArgs e)
        {
            if (_sw.IsRunning) stop();
            else start();
            FrameVisible(!_sw.IsRunning);
        }

        void start()
        {
            _udp.RemoteHost = tbHost.Text;
            _sw.Start();
            btnStartStopr.Text = "Stop";
        }

        void stop()
        {
            _sw.Stop();
            btnStartStopr.Text = "Start";
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Enabled = false;

            updateBmp();
            var bmp = formCaptureBox.CaptureFrame();
            _gResizeBmp.DrawImage(bmp, 0, 0, _resizeBmp.Width, _resizeBmp.Height);
            pbPreview.Image = _resizeBmp;
            pbPreview.Invalidate();

            if (_sw.IsRunning) udpJPGSend();
            timer1.Enabled = true;
        }
        
        private void udpJPGSend()
        {
            EncoderParameters encParams = new EncoderParameters(1);
            int restHeight = _resizeBmp.Height;
            int divide = Math.Max(1, (int)nudDivide.Value);
            byte y = 0;
            int count = 0;
            int hei = Math.Max(4,((_resizeBmp.Height+3) / divide)) & 0xFFFC;

            long msec = _sw.ElapsedMilliseconds;
            do
            {
                if (++count == divide) {
                    hei = restHeight;
                } else {
                    hei = Math.Min(restHeight, Math.Max(8, (restHeight / (divide + 1 - count)) & 0xFFF8));
                }
                _jpgQuality = Math.Min(100, _jpgQuality + 2);

                if (_jpgBuf == null || _jpgBuf.Width != _resizeBmp.Width || _jpgBuf.Height != hei)
                {
                    if (_jpgBuf != null && _jpgBuf.Height != hei)
                    {
                        if (_jpgBuf.Height < hei) {
                            _jpgQuality = Math.Max(0, _jpgQuality - 5);
                        } else {
                            _jpgQuality = Math.Min(100, _jpgQuality + 5);
                        }
                    }
                    _gJpgBuf?.Dispose();
                    _jpgBuf?.Dispose();
                    _jpgBuf = new Bitmap(_resizeBmp.Width, hei, PixelFormat.Format24bppRgb);
                    _gJpgBuf = Graphics.FromImage(_jpgBuf);
                }
                _gJpgBuf.DrawImage(_resizeBmp, 0, -y, _resizeBmp.Width, _resizeBmp.Height);
                byte[] rgbValues = { 0 };
                for (;;)
                {
                    using (MemoryStream ms = new MemoryStream())
                    {
                        ms.WriteByte(y);
                        ms.WriteByte((byte)_resizeBmp.Height);
                        ms.WriteByte((byte)(_resizeBmp.Width & 0xff));
                        ms.WriteByte((byte)(_resizeBmp.Width >> 8));
                        encParams.Param[0] = new EncoderParameter(System.Drawing.Imaging.Encoder.Quality, _jpgQuality);
                        _jpgBuf.Save(ms, _jpgEncoder, encParams);
                        rgbValues = ms.GetBuffer();
                    }
                    if (rgbValues.Length <= 1460) break;
                    if (_jpgQuality <= 0) break;
                    _jpgQuality = Math.Max(0, _jpgQuality - 2 - ((rgbValues.Length - 1460)>>4));
                }
                if (rgbValues.Length <= 1460)
                {
                    while (_sw.IsRunning && msec + count * nudDelay.Value > _sw.ElapsedMilliseconds) { System.Threading.Thread.Sleep(0); }
                    _udp.send(rgbValues);
                }
                y += (byte)_jpgBuf.Height;
                restHeight -= _jpgBuf.Height;
            } while (restHeight > 0);
        }

        private void udpBMPSend()
        {
            System.Drawing.Imaging.BitmapData bmpData =
                _resizeBmp.LockBits(new Rectangle(0, 0, _resizeBmp.Width, _resizeBmp.Height)
                            , System.Drawing.Imaging.ImageLockMode.ReadWrite
                            , PixelFormat.Format16bppRgb565);

            IntPtr ptr = bmpData.Scan0;
            int restHeight = _resizeBmp.Height;
            byte y = 0;
            int count = 0;
            long msec = _sw.ElapsedMilliseconds;
            do
            {
                while (_sw.IsRunning && msec + count * 12 > _sw.ElapsedMilliseconds) { System.Threading.Thread.Sleep(0); }
                int hei = Math.Min(restHeight, 730 / _resizeBmp.Width);
                int bytes = _resizeBmp.Width * hei * 2;
                byte[] rgbValues = new byte[bytes + 4];
                Marshal.Copy(ptr + _resizeBmp.Width * y * 2, rgbValues, 4, bytes);
                rgbValues[0] = y;
                rgbValues[1] = (byte)hei;
                rgbValues[2] = (byte)(_resizeBmp.Width & 0xff);
                rgbValues[3] = (byte)(_resizeBmp.Width >> 8);
                _udp.send(rgbValues);
                count++;
                y += (byte)hei;
                restHeight -= hei;
            } while (restHeight > 0);
            _resizeBmp.UnlockBits(bmpData);
        }
    }
}
