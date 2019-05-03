#ifndef _UDPRECIEVER_H_
#define _UDPRECIEVER_H_

#include <vector>
#include <M5Stack.h>
#include <M5TreeView.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <rom/tjpgd.h>
#include <driver/spi_master.h>
/*
#define jpgColor(c)                                \
  (((uint16_t)(((uint8_t *)(c))[0] & 0xF8) << 8) | \
   ((uint16_t)(((uint8_t *)(c))[1] & 0xFC) << 3) | \
   ((((uint8_t *)(c))[2] & 0xF8) >> 3))

static uint16_t jpgColor(uint8_t r, uint8_t g, uint8_t b) {
  return (r & 0xF8) | (b & 0xF8) << 5 | (g & 0xE0) >> 5 | (g & 0x1C) << 11;
}
*/

#define jpgColor(r, g, b) \
    (uint16_t)(((r) & 0xF8) | ((b) & 0xF8) << 5 | ((g) & 0xE0) >> 5 | ((g) & 0x1C) << 11)



typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t maxWidth;
  uint16_t maxHeight;
  uint16_t offX;
  uint16_t offY;
  jpeg_div_t scale;
  const void *src;
  size_t len;
  size_t index;
  M5Display *tft;
  uint16_t outWidth;
  uint16_t outHeight;
} jpg_file_decoder_t;


class UDPReceiver
{
public:
  UDPReceiver() {}

  virtual void operator()(MenuItem* mi) {
    M5TreeView* treeView = ((M5TreeView*)(mi->topItem()));
    M5.Lcd.fillScreen(0);

    M5.Lcd.setTextColor(0xFFFF, 0);
    M5.Lcd.drawString("WiFiwaiting", 0, 0);
    if (mi->tag == 0) {
      WiFi.mode(WIFI_MODE_STA);
      WiFi.begin();
      while( WiFi.status() != WL_CONNECTED) {
        delay(500);
      }
      M5.Lcd.drawString(WiFi.localIP().toString(), 0, 0);
      _udp.begin(WiFi.localIP(), 63333);
    } else {
      WiFi.mode(WIFI_MODE_AP);
      WiFi.begin();
      M5.Lcd.drawString(WiFi.softAPIP().toString(), 0, 0);
      _udp.begin(WiFi.softAPIP(), 63333);
    }

    if (setup()) {
      while (treeView->checkInput() != M5TreeView::eCmd::BACK && loop());
      close();
    }
    M5.Lcd.fillScreen(MenuItem::backgroundColor);
  }
  bool setup()
  {
    for (int i = 0; i < 6; ++i) { _sent[i] = false; }
    if (_spi == NULL)  _spi = spi_start();
//*
    for (int i = 0; i < UDP_QUEUE_COUNT; ++i) {
      _flgQueue[i] = false;
    }
    _isRunning = true;
    _lastX = -1;
    _lastY = -1;
    _lastW = -1;
    _lastH = -1;

    //disableCore0WDT();
    //disableCore1WDT();
    xTaskCreatePinnedToCore(taskReceive, "taskReceive", 4096, this, 1, NULL, 0);
//*/
    return true;
  }
  void close()
  {
    _isRunning = false;
    send_framebuffer_finish(_spi);
    delay(10);
  }

  bool loop()
  {
//*
    for (int i = 0; i < UDP_QUEUE_COUNT; ++i) {
      if (_flgQueue[i]) {
        udpJpg(_udpQueue[i]);
        _flgQueue[i] = false;
      }
    }
/*
    while (!_udpQueue.empty()) {
      if (M5.BtnC.isReleased()) udpJpg(_udpQueue.front());
      _udpQueue.pop();
    }
//
    if (_udp.parsePacket()) {
      _udpbuf.resize(UDP_BUF_LEN);
      _udpbuf.resize(_udp.read(&_udpbuf[0], UDP_BUF_LEN));
      if (!_udpbuf.empty()) {
        ++_count;
        udpJpg(_udpbuf);
      }
      if (_sec != millis() / 1000) {
        //Serial.printf("%d packet/s : %d drop/s\r\n", count, drop);
        Serial.printf("%d packet/s\r\n", _count);
        _sec = millis() / 1000;
        _count = 0;
      }
    }
//*/
    return true;
  }
private:
  enum
  { UDP_BUF_LEN = 1460
  , DMA_BUF_LEN = 10240
  , UDP_QUEUE_COUNT = 3
  };
  WiFiUDP _udp;
  uint16_t _count;
  uint32_t _sec;
//*
  volatile bool _isRunning;
  volatile bool _flgQueue[UDP_QUEUE_COUNT];
  std::vector<uint8_t> _udpQueue[UDP_QUEUE_COUNT];
  uint8_t _idxQueue = 0;

  static bool _sent[6];
  static uint16_t _pixBuf[2][DMA_BUF_LEN];
  static spi_device_handle_t _spi;
  static int _lastX, _lastY, _lastW, _lastH;

  static void taskReceive(void* arg) {
    UDPReceiver* me = (UDPReceiver*)arg;
    uint16_t count = 0;
    uint16_t drop = 0;
    uint32_t sec = 0;
    uint8_t idx = 0;

    while (me->_isRunning) {
      if (me->_udp.parsePacket()) {
        if (me->_flgQueue[idx]) {
          ++drop;
          me->_udp.flush();
        } else {
          ++count;
          me->_udpQueue[idx].resize(UDP_BUF_LEN);
          me->_udpQueue[idx].resize(me->_udp.read(&me->_udpQueue[idx][0], UDP_BUF_LEN));
          if (!me->_udpQueue[idx].empty()) {
            me->_flgQueue[idx] = true;
            idx = (1+idx) % UDP_QUEUE_COUNT;
          }
        }
      }
      if (sec != millis() / 1000) {
        Serial.printf("draw %d packet/s : drop %d packet/s\r\n", count, drop);
        sec = millis() / 1000;
        count = 0;
        drop = 0;
      }
      delay(1);
    }
    vTaskDelete(NULL);
  } 
  
//*/
  static void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
  {
    int dc = (int)t->user;
    gpio_set_level((gpio_num_t)TFT_DC_PIN, dc);
  }

  spi_device_handle_t spi_start()
  {
    esp_err_t ret;
    spi_device_handle_t hSpi;
    spi_bus_config_t buscfg = {
        .mosi_io_num = TFT_MOSI_PIN,
        .miso_io_num = TFT_MISO_PIN,
        .sclk_io_num = TFT_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DMA_BUF_LEN * 2 + 8,
        .flags = 0,
        .intr_flags = 0};
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0, //SPI mode 0
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = 40 * 1000 * 1000,
        .input_delay_ns = 0,
        .spics_io_num = TFT_CS_PIN, //CS pin
        .flags = 0,
        .queue_size = 7,                         //We want to be able to queue 7 transactions at a time
        .pre_cb = lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
        .post_cb = 0};
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &hSpi);
    ESP_ERROR_CHECK(ret);
    return hSpi;
  }

  static void send_framebuffer(spi_device_handle_t hSpi, int x, int y, int w, int h, uint16_t *framebuffer)
  {
    esp_err_t ret;
    static spi_transaction_t trans[6];
    spi_transaction_t *rtrans;
    for (int i = 0; i < 6; i++)
    {
      if (_sent[i]) {
        ret = spi_device_get_trans_result(hSpi, &rtrans, portMAX_DELAY);
        assert(ret == ESP_OK);
        _sent[i] = false;
      }
      memset(&trans[i], 0, sizeof(spi_transaction_t));
      if ((i & 1) == 0)
      {
          trans[i].length = 8;
          trans[i].user = (void *)0;
          trans[i].tx_data[0] = 0x2A + (i>>1);
      } else {
          trans[i].length = 8 * 4;
          trans[i].user = (void *)1;
      }
      trans[i].flags = SPI_TRANS_USE_TXDATA;
      switch (i) {
      case 1:
        trans[1].tx_data[0] = x >> 8;             //Start Col High
        trans[1].tx_data[1] = x & 0xFF;           //Start Col Low
        trans[1].tx_data[2] = (x + w - 1) >> 8;   //End Col High
        trans[1].tx_data[3] = (x + w - 1) & 0xFF; //End Col Low
      case 0:
        if (_lastX == x && _lastW == w) continue;
        break;

      case 3:
        trans[3].tx_data[0] = y >> 8;             //Start page high
        trans[3].tx_data[1] = y & 0xFF;           //start page low
        trans[3].tx_data[2] = (y + h - 1) >> 8;   //end page high
        trans[3].tx_data[3] = (y + h - 1) & 0xFF; //end page low
      case 2:
        if (_lastY == y && _lastH == h) continue;
        break;

      case 5:
        trans[5].tx_buffer = framebuffer;         //finally send the line data
        trans[5].length = w * 2 * 8 * h;          //Data length, in bits
        trans[5].flags = 0;                       //undo SPI_TRANS_USE_TXDATA flag
        break;
      }
      ret = spi_device_queue_trans(hSpi, &trans[i], portMAX_DELAY);
      assert(ret == ESP_OK);
      _sent[i] = true;
    }
    _lastY = y;
    _lastH = h;
  }

  static void send_framebuffer_finish(spi_device_handle_t hSpi)
  {
    spi_transaction_t *rtrans;
    esp_err_t ret;
    for (int i = 0; i < 6; ++i)
    {
      if (!_sent[i]) continue;
      ret = spi_device_get_trans_result(hSpi, &rtrans, portMAX_DELAY);
      assert(ret == ESP_OK);
    }
  }

  static uint32_t jpgRead(JDEC *decoder, uint8_t *buf, uint32_t len) {
    jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
    if (buf) {
      memcpy(buf, (const uint8_t *)jpeg->src + jpeg->index, len);
    }
    jpeg->index += len;
    return len;
  }

  static uint32_t jpgWrite(JDEC *decoder, void *bitmap, JRECT *rect) {
    jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
    if ((rect->right <  jpeg->offX)
     || (rect->left >= (jpeg->offX + jpeg->outWidth))
     || (rect->bottom < jpeg->offY)
     || (rect->top >=  (jpeg->offY + jpeg->outHeight))) {
      return 1;
    }

    uint16_t x = rect->left;
    uint16_t y = rect->top;
    uint16_t w = rect->right + 1 - x;
    uint16_t h = rect->bottom + 1 - y;
    uint8_t *data = (uint8_t *)bitmap;

    if (rect->top < jpeg->offY) {
      uint16_t linesToSkip = jpeg->offY - rect->top;
      data += linesToSkip * w * 3;
      h -= linesToSkip;
      y += linesToSkip;
    }
    if (rect->bottom >= (jpeg->offY + jpeg->outHeight)) {
      uint16_t linesToSkip = (rect->bottom + 1) - (jpeg->offY + jpeg->outHeight);
      h -= linesToSkip;
    }
    uint16_t oL = (rect->left < jpeg->offX) ? jpeg->offX - rect->left : 0;
    uint16_t oR = (rect->right >= (jpeg->offX + jpeg->outWidth)) ? (rect->right + 1) - (jpeg->offX + jpeg->outWidth) : 0;

    static uint8_t pixFlip = 0;
    uint16_t pixIndex;
    uint16_t line;
    uint16_t hh = h;
    uint16_t ww = w - (oL + oR);
    uint8_t r, g, b;
    uint16_t* p = _pixBuf[pixFlip];
    uint16_t yy = 0;
    while (h--) {
      data += 3 * oL;
      line = ww;
      pixIndex = x * 2 + (++yy * 2 - 1) * jpeg->maxWidth * 2;
      //pixIndex += jpeg->maxWidth * 2;
      while (line--) {
        r = data[0];
        g = data[1];
        b = data[2];
        p[pixIndex- jpeg->maxWidth * 2] = jpgColor( r                     ,  g                     ,  b                     );
        p[pixIndex++]     = jpgColor((r < 251) ? r + 4 : 255, (g < 253) ? g + 2 : 255, (b < 251) ? b + 4 : 255);
        p[pixIndex- jpeg->maxWidth * 2] = jpgColor((r < 249) ? r + 6 : 255, (g < 252) ? g + 3 : 255, (b < 249) ? b + 6 : 255);
        p[pixIndex++]     = jpgColor((r < 253) ? r + 2 : 255, (g < 254) ? g + 1 : 255, (b < 253) ? b + 2 : 255);
        data += 3;
      }
      data += 3 * oR;
    }

    if (x + ww >= jpeg->maxWidth) {
      send_framebuffer(_spi
                      , (- jpeg->offX + jpeg->x + oL * 2)
                      , (y * 2 - jpeg->offY + jpeg->y)
                      , jpeg->maxWidth * 2  // ww * 2
                      , hh * 2
                      , p);
      pixFlip = 1 - pixFlip;
    }
    return 1;
  }

  static bool jpgDecode(jpg_file_decoder_t *jpeg, uint32_t (*reader)(JDEC *, uint8_t *, uint32_t)) {
    static uint8_t work[3100];
    JDEC decoder;

    JRESULT jres = jd_prepare(&decoder, reader, work, 3100, jpeg);
    if (jres != JDR_OK) {
      log_e("jd_prepare failed! %s", jd_errors[jres]);
      return false;
    }

    uint16_t jpgWidth = decoder.width / (1 << (uint8_t)(jpeg->scale));
    uint16_t jpgHeight = decoder.height / (1 << (uint8_t)(jpeg->scale));

    if (jpeg->offX >= jpgWidth || jpeg->offY >= jpgHeight) {
      log_e("Offset Outside of JPEG size");
      return false;
    }

    size_t jpgMaxWidth = jpgWidth - jpeg->offX;
    size_t jpgMaxHeight = jpgHeight - jpeg->offY;

    jpeg->outWidth = (jpgMaxWidth > jpeg->maxWidth) ? jpeg->maxWidth : jpgMaxWidth;
    jpeg->outHeight = (jpgMaxHeight > jpeg->maxHeight) ? jpeg->maxHeight : jpgMaxHeight;

    jres = jd_decomp(&decoder, jpgWrite, (uint8_t)jpeg->scale);
    if (jres != JDR_OK) {
      log_e("jd_decomp failed! %s", jd_errors[jres]);
      return false;
    }

    return true;
  }

  void udpJpg(const std::vector<uint8_t> &udpbuf) {
    int w = (uint16_t)udpbuf[2];// + ((uint16_t)udpbuf[3] << 8);
    int y = udpbuf[0];
    int h = udpbuf[1];

    jpg_file_decoder_t jpeg;

    jpeg.src = &udpbuf[4];
    jpeg.len = udpbuf.size() - 4;
    jpeg.index = 0;
    jpeg.x = 160 - w;
    jpeg.y = 120 - h + y * 2;
    jpeg.maxWidth = w;
    jpeg.maxHeight = 120;
    jpeg.offX = 0;
    jpeg.offY = 0;
    jpeg.scale = JPEG_DIV_NONE;
    jpeg.tft = &M5.Lcd;

    jpgDecode(&jpeg, jpgRead);
  }
};
spi_device_handle_t UDPReceiver::_spi = NULL;
uint16_t UDPReceiver::_pixBuf[2][DMA_BUF_LEN];
bool UDPReceiver::_sent[6];
int UDPReceiver::_lastX, UDPReceiver::_lastW;
int UDPReceiver::_lastY, UDPReceiver::_lastH;

#endif
