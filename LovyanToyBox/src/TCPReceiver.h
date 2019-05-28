#ifndef _TCPRECEIVER_H_
#define _TCPRECEIVER_H_

#include <M5Stack.h>
#include <M5TreeView.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <driver/spi_master.h>
#include <esp_heap_alloc_caps.h>
//#include <rom/tjpgd.h>
//#include "tjpgd.h"
#include "tjpgdClass.h"
#include "DMADrawer.h"

#define dmaColor(r, g, b) \
    (uint16_t)(((uint8_t)(r) & 0xF8) | ((uint8_t)(b) & 0xF8) << 5 | ((uint8_t)(g) & 0xE0) >> 5 | ((uint8_t)(g) & 0x1C) << 11)

#define BYTECLIP(v) Clip8[(uint16_t)(v)]

static const uint8_t Clip8[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
  64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
  96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
  128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
  160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
  192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
  224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
};


class TCPReceiver
{
  typedef struct {
    uint16_t x;
    uint16_t y;
    const void *src;
    DMADrawer *dma;
    size_t index;
    uint8_t magnify;
  } jpg_param_t;


public:
  TCPReceiver() {}

  M5TreeView::eCmd cmd;
  virtual void operator()(MenuItem* mi) {
    M5TreeView* treeView = ((M5TreeView*)(mi->topItem()));
    _softap = mi->tag != 0;
    M5.Lcd.fillScreen(0);
    M5.Lcd.setTextColor(0xFFFF, 0);
    M5.Lcd.drawString("WiFiwaiting", 0, 0);

    if (setup()) {
      do {
        cmd = treeView->checkInput();
      } while (cmd != M5TreeView::eCmd::BACK && loop());
      close();
    }
    M5.Lcd.fillScreen(MenuItem::backgroundColor);
  }
  bool setup()
  {
    Serial.println("setup");
    for (int i = 0; i < QUEUE_COUNT; ++i) {
      _flgQueue[i] = false;
      _tcpQueue[i] = (uint8_t*)pvPortMallocCaps(JPG_BUF_LEN, MALLOC_CAP_8BIT);
    }

    tcpStart();
    if (!_softap) {
      M5.Lcd.drawString(WiFi.localIP().toString(), 0, 0);
    } else {
      M5.Lcd.drawString(WiFi.softAPIP().toString(), 0, 0);
    }

    _isRunning = true;
    Dma.setup(DMA_BUF_LEN);

    disableCore0WDT();
    disableCore1WDT();
    xTaskCreatePinnedToCore(taskReceive, "taskReceive", 4096, this, 1, NULL, 0);
    //decoder.jd_multitask_begin();

    return true;
  }
  void close()
  {
    _isRunning = false;
    delay(10);
    _tcp.available().stop();
    _tcp.stop();
    Dma.close();
    for (int i = 0; i < QUEUE_COUNT; ++i) {
      _flgQueue[i] = false;
      free(_tcpQueue[i]);
    }
    //decoder.jd_multitask_end();
  }

  bool loop()
  {
    if (cmd == M5TreeView::eCmd::ENTER) {
      _wifi_stage = 2;
    }

    if (_flgQueue[_queueIndex]) {
      if (M5.BtnC.isPressed()) {  // DEBUG
        uint32_t ms = micros();
        drawJpg(_tcpQueue[_queueIndex]);
        ms = micros() - ms;
        Serial.printf("draw:%d\r\n", ms);
      } else {
        drawJpg(_tcpQueue[_queueIndex]);
      }
      _flgQueue[_queueIndex] = false;
      _queueIndex = (1 + _queueIndex) % QUEUE_COUNT;
    }

    return true;
  }
private:
  enum
  { DMA_BUF_LEN = 10240   // 320x32 pixel
  , JPG_BUF_LEN = 65536
  , QUEUE_COUNT = 2
  };

  WiFiServer _tcp;
  JDEC decoder;
  uint8_t _wifi_stage;

  volatile bool _isRunning;
  volatile bool _flgQueue[QUEUE_COUNT];
  uint8_t* _tcpQueue[QUEUE_COUNT];
  uint8_t _queueIndex = 0;
  bool _softap = false;

  DMADrawer Dma;

  static void taskReceive(void* arg) {
    TCPReceiver* me = (TCPReceiver*)arg;
    uint16_t retry = 0;
    uint16_t size;
    uint16_t count = 0;
    uint8_t idxQueue = 0;
    int pos = 0;
    int readsize = 0;
    long totalSize = 0;
    uint32_t sec = 0;
    bool flg_error = false;
    WiFiClient client;
    uint8_t sequence = 0;

    while (me->_isRunning) {
      if (me->_wifi_stage == 2) {
        me->_tcp.end();
        me->tcpStart();
      }
      if (client.connected()) {
        if (!me->_flgQueue[idxQueue]) {

          switch (sequence) {
          case 0:

            if (client.available()) {
              Serial.println("broken data received");
              client.read(me->_tcpQueue[idxQueue], JPG_BUF_LEN);
              delay(1);
              break;
            }
            ++sequence;

          case 1:   // data request to client
            client.write('\n');
            delay(1);
            retry = 1000;
            ++sequence; 

          case 2:   // data receive wait
            if (2 > client.available()) {
              if (--retry) {
                delay(1);
              } else {
                Serial.println("data wait timeout");
                flg_error = true;
              }
              break;
            }
            ++sequence; 

          case 3:   // read data size
            if (2 == client.read((uint8_t*)&size, 2)) {
              totalSize += 2;
              if (size < 100) {
                Serial.printf("size too small: %d\r\n", size);
                Serial.printf("available: %d\r\n", client.available());
                flg_error = true;
                break;
              } else if (JPG_BUF_LEN < size) {
                Serial.printf("size too large: %d\r\n", size);
                flg_error = true;
                break;
              }
            }
            pos = 0;
            retry = 1000;
            ++sequence;

          case 4:
            readsize = client.read(&me->_tcpQueue[idxQueue][pos], size);
            if (0 >= readsize) {
              if (--retry) {
                delay(1);
              } else {
                Serial.println("read timeout.");
                flg_error = true;
              }
              break;
            }
            totalSize += readsize;
            size -= readsize;
            if (size) {
              pos += readsize;
              retry = 2000;
              break;
            }

            me->_flgQueue[idxQueue] = true;
            idxQueue = (1 + idxQueue) % QUEUE_COUNT;
            ++count;
            sequence = 0;
            break;
          }

          if (flg_error) {
            client.flush();
            flg_error = false;
            sequence = 0;
          }
        } else {
          delay(1);
        }
      } else {
        client = me->_tcp.available();
        if (client.connected()) {
          Serial.println("tcp connect");
          sequence = 0;
        } else {
          delay(10);
        }
      }
      if (sec != millis() / 1000) {
        Serial.printf("%d fps  %d Byte \r\n", count, totalSize);
        sec = millis() / 1000;
        count = 0;
        totalSize = 0;
      }
    }
    vTaskDelete(NULL);
  } 

  void tcpStart(void) {
    _wifi_stage = 0;
    Serial.println("wifi init");
    if (!_softap) {
      WiFi.mode(WIFI_MODE_STA);
      for (int i = 0; i < 10; ++i) {
        WiFi.disconnect(true);
        delay(100);
        WiFi.begin();
        for (int j = 0; j < 100; ++j) {
          if (WiFi.status() != WL_CONNECTED) {
            delay(50);
          }
        }
        if (WiFi.status() == WL_CONNECTED) {
          _tcp.setNoDelay(true);
          _tcp.begin(63333);
          Serial.println("WiFi Connected.");
          break;
        }
      }
    } else {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_MODE_AP);
      WiFi.begin();
      _tcp.setNoDelay(true);
      _tcp.begin(63333);
    }
  }

  static uint16_t jpgRead(JDEC *decoder, uint8_t *buf, uint16_t len) {
    jpg_param_t *jpeg = (jpg_param_t *)decoder->device;
    if (buf) {
      memcpy(buf, (const uint8_t *)jpeg->src + jpeg->index, len);
    }
    jpeg->index += len;
    return len;
  }

  static uint16_t jpgWrite(JDEC *decoder, void *bitmap, JRECT *rect) {
    jpg_param_t *jpeg = (jpg_param_t *)decoder->device;
    uint16_t width = decoder->width;
    uint16_t x = rect->left;
    uint16_t y = rect->top;
    uint16_t w = rect->right + 1 - x;
    uint16_t h = rect->bottom + 1 - y;
    uint16_t* p = jpeg->dma->getNextBuffer();
    uint16_t* dst;

    uint8_t *data = (uint8_t*)bitmap;
    uint8_t magnify = jpeg->magnify;
    uint8_t line;
    uint8_t ww = w;
    uint8_t hh = h;
    uint8_t yy = 0;

    if (!magnify) {
      while (hh--) {
        line = ww;
        dst = p + x + width * yy;
        while (line--) {
          *dst++ = dmaColor(data[0], data[1], data[2]);
          data += 3;
        }
        ++yy;
      }
    } else {
      uint16_t addy = width << 1;
      uint8_t r, g, b;
      while (hh--) {
        line = ww;
        dst = p + ((x + addy * yy) << 1);
        while (line--) {
          r = *data++;
          g = *data++;
          b = *data++;
          *dst++ = dmaColor(r,  g,  b);
          *dst-- = dmaColor(BYTECLIP(r + 4), BYTECLIP(g + 2), BYTECLIP(b + 4));
          dst += addy;
          *dst++ = dmaColor(BYTECLIP(r + 6), BYTECLIP(g + 3), BYTECLIP(b + 6));
          *dst++ = dmaColor(BYTECLIP(r + 2), BYTECLIP(g + 1), BYTECLIP(b + 2));
          dst -= addy;
        }
        ++yy;
      }
    }

    if (x + w >= width) {
      jpeg->dma->draw( jpeg->x
                     , jpeg->y + (y << magnify)
                     , width << magnify
                     , h     << magnify
                     );
    }
    return 1;
  }

  bool drawJpg(uint8_t* udpbuf) {
    jpg_param_t jpeg;

    jpeg.src = &udpbuf[0];
    jpeg.index = 0;
    jpeg.dma = &Dma;

    JRESULT jres = decoder.jd_prepare(jpgRead, &jpeg);
    if (jres != JDR_OK) {
      log_e("jd_prepare failed! %s", jd_errors[jres]);
      return false;
    }

    if (decoder.width > 160 || decoder.height > 120) {
      jpeg.magnify = 0;
      jpeg.x = 160 - decoder.width/2;
      jpeg.y = 120 - decoder.height/2;
    } else {
      jpeg.magnify = 1;
      jpeg.x = 160 - decoder.width;
      jpeg.y = 120 - decoder.height;
    }
//    jres = decoder.jd_decomp_multitask(jpgWrite, JPEG_DIV_NONE);
    jres = decoder.jd_decomp(jpgWrite, JPEG_DIV_NONE);
    if (jres != JDR_OK) {
      log_e("jd_decomp failed! %s", jd_errors[jres]);
      return false;
    }

    return true;
  }
};

#endif
