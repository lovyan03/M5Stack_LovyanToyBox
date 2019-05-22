using System;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace ScreenShotSender
{
    public class TCPSender
    {
        private Task _task;
        private string _addr;
        private int _port;
        private byte[] _sendBuffer;
        private CancellationTokenSource _cts;

        public TCPSender()
        {
        }

        public void start(string addr, int port)
        {
            _addr = addr;
            _port = port;
            _cts = new CancellationTokenSource();
            _task = Task.Run(() => { TaskClient(_cts.Token); });
        }

        public void stop()
        {
            _cts?.Cancel();
        }


        public void setData(byte[] src)
        {
            _sendBuffer = src;
        }

        private void TaskClient(CancellationToken ct)
        {
            byte[] resBytes = new byte[256];
            while (!ct.IsCancellationRequested)
            {
                try
                {
                    var client = new TcpClient(_addr, _port);
                    var ns = client.GetStream();
                    ns.ReadTimeout = 10000;
                    ns.WriteTimeout = 10000;
                    while (!ct.IsCancellationRequested && client.Connected)
                    {
                        try
                        {
                            if (0 < ns.Read(resBytes, 0, resBytes.Length))
                            {
                                var tmp = _sendBuffer;
                                ns.Write(tmp, 0, tmp.Length);
                            }
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }
                    ns.Close();
                    client.Close();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
            }
        }
    }
}