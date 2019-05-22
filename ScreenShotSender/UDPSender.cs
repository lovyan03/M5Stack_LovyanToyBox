using System.Net.Sockets;

namespace ScreenShotSender
{
    public class UDPSender
    {
        private UdpClient _udpForSend; // 送信用クライアント
        private string _addr;          // 送信先アドレス
        private int _port;             // 送信先のポート

        public UDPSender()
        {
        }

        public bool start(string addr, int port)
        {
            try
            {
                _udpForSend?.Dispose();
                _udpForSend = new UdpClient(0); // 送信元ポート0(未使用ポートが割当てられる)
                _addr = addr;
                _port = port;
                var socket = new System.Net.Sockets.Socket(SocketType.Dgram,ProtocolType.Udp);
                socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendBuffer, 1024*1024);
                return true;
            }
            catch
            {
                return false;
            }
        }

        public void send(byte[] sendBytes)
        {
            try
            {
                _udpForSend.SendAsync(sendBytes, sendBytes.Length, _addr, _port);
            }
            catch { }
        }

        public void stop()
        {
            try
            {
                _udpForSend.Close();
            }
            catch { }
        }
    }
}