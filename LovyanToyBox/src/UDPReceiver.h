#ifndef _UDPRECIEVER_H_
#define _UDPRECIEVER_H_

#include <M5Stack.h>
#include <vector>
#include <MenuCallBack.h>
#include <WiFi.h>
#include <WiFiUdp.h>

class UDPReceiver : public MenuCallBack
{


  WiFiUDP udp;
  enum
  { UDP_BUF_LEN = 1460
  , DRAW_WIDTH  = 240
  , DRAW_HEIGHT = 160
  };
  uint8_t udpbuf[UDP_BUF_LEN];
  std::vector<uint16_t> bmpbuf;
public:
  UDPReceiver() {}
  bool setup()
  {
    M5.Lcd.setTextColor(0xFFFF, 0);
    M5.Lcd.drawString("WiFiwaiting", 0, 0);
    WiFi.begin();
    while( WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    M5.Lcd.drawString(WiFi.localIP().toString(), 0, 0);
    udp.begin(WiFi.localIP(), 63333);

    bmpbuf.resize(DRAW_WIDTH * DRAW_HEIGHT);
    return true;
  }
  void close()
  {
  }
  uint16_t count = 0;

  bool loop()
  {
    size_t rsize;
    if (udp.parsePacket()) {
      rsize = udp.read((uint8_t*)udpbuf, UDP_BUF_LEN);
      if (rsize) {
        ++count;
        udpJpg(rsize);
      }
    } 
    return true;
  }
  void udpJpg(uint16_t rsize) {
    int w = (uint16_t)udpbuf[2];// + ((uint16_t)udpbuf[3] << 8);
    int y = udpbuf[0];
    int h = udpbuf[1];

    M5.Lcd.drawJpg((uint8_t*)(udpbuf + 4), rsize - 4, 40, 30 + y);
  }

  int _py = 0;
  void udpBmp() {
    int y = udpbuf[0];
    int h = udpbuf[1];
    int w = (uint16_t)udpbuf[2];// + ((uint16_t)udpbuf[3] << 8);

    if (y <= _py) {
      //M5.Lcd.drawBitmap(40, 30, DRAW_WIDTH, DRAW_HEIGHT, (uint16_t*)(&bmpbuf[0]));
      M5.Lcd.drawBitmap(40, 30, w * 2, 160, (uint16_t*)(&bmpbuf[0]));
    }
    uint16_t color;
    for (int i = 0; i < w; ++i) {
      for (int j = 0; j < h; ++j) {
        color = ((uint16_t*)udpbuf)[2 + (j * w + i)];
        bmpbuf[(((j+y)*2  ) * w + i) * 2    ] = color;
        bmpbuf[(((j+y)*2+1) * w + i) * 2    ] = color;
        bmpbuf[(((j+y)*2  ) * w + i) * 2 + 1] = color;
        bmpbuf[(((j+y)*2+1) * w + i) * 2 + 1] = color;
      }
    }
    //memcpy(&bmpbuf[w * y], udpbuf + 4, w * h * 2);
    //M5.Lcd.drawBitmap(40, 30 + y, w, h, (uint16_t*)(udpbuf + 4));
    ++count;
    M5.Lcd.drawString(String(count), 0, 0, 0);
    _py = y;
  }
};

#endif
