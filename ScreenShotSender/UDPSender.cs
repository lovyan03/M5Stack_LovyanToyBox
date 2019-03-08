using System.Net;//for UDP
using System.Net.Sockets; //for UDP
using System.Threading;//for Interlocked

namespace ProptotypeControler //namespaceは本体に合わせて要修正
{
    public class UDPSender
    {
        private UdpClient udpForSend; //送信用クライアント
        private int remotePort;//送信先のポート

        public string RemoteHost { get; set; } = "localhost"; //送信先のIPアドレス

        public UDPSender()
        {
        }

        public bool init(int port_snd, int port_to) //UDP設定（送受信用ポートを開きつつ受信用スレッドを生成）
        {
            try
            {
                udpForSend = new UdpClient(port_snd); //送信用ポート
                remotePort = port_to; //送信先ポート
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
                udpForSend.SendAsync(sendBytes, sendBytes.Length, RemoteHost, remotePort);
            }
            catch { }
        }

        public void end() //送受信用ポートを閉じつつ受信用スレッドも廃止
        {
            try
            {
                udpForSend.Close();
            }
            catch { }
        }
    }
}