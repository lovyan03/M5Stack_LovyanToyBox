
#pragma GCC optimize ("O3")

#include "DMADrawer.h"

#include <M5Stack.h>
#include <esp_heap_alloc_caps.h>
#include <driver/spi_master.h>

static spi_device_handle_t _spi = NULL;

static void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
  int dc = (int)t->user;
  gpio_set_level((gpio_num_t)TFT_DC_PIN, dc);
}

static spi_device_handle_t spi_start(uint16_t len)
{
  esp_err_t ret;
  spi_device_handle_t hSpi;
  spi_bus_config_t buscfg = {
      .mosi_io_num = TFT_MOSI_PIN,
      .miso_io_num = TFT_MISO_PIN,
      .sclk_io_num = TFT_CLK_PIN,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = len * 2 + 8,
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

bool DMADrawer::setup(uint32_t buflen)
{
  _buflen = buflen;
  for (int i = 0; i < 2; ++i) 
    _pixBuf[i] = (uint16_t*)pvPortMallocCaps(buflen * sizeof(uint16_t), MALLOC_CAP_DMA);
  _lastX = -1;
  _lastY = -1;
  _lastW = -1;
  _lastH = -1;
  for (int i = 0; i < 6; ++i) { _sent[i] = false; }
  if (_spi == NULL)  _spi = spi_start(buflen);
  return true;
}

void DMADrawer::close()
{
  draw_finish();
  for (int i = 0; i < 2; ++i) 
    free(_pixBuf[i]);
}

uint16_t* DMADrawer::getNextBuffer() { 
  return _pixBuf[_pixFlip];
}

void DMADrawer::draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  esp_err_t ret;
  static spi_transaction_t trans[6];
  spi_transaction_t *rtrans;
  for (uint8_t i = 0; i < 6; i++)
  {
    if (_sent[i]) {
      ret = spi_device_get_trans_result(_spi, &rtrans, portMAX_DELAY);
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
    case 0:
      if (_lastX == x && _lastW == w) continue;
      break;

    case 1:
      if (_lastX == x && _lastW == w) continue;
      trans[1].tx_data[0] = x >> 8;             //Start Col High
      trans[1].tx_data[1] = x & 0xFF;           //Start Col Low
      trans[1].tx_data[2] = (x + w - 1) >> 8;   //End Col High
      trans[1].tx_data[3] = (x + w - 1) & 0xFF; //End Col Low
      break;

    case 2:
      if (_lastY == y && _lastH == h) continue;
      break;

    case 3:
      if (_lastY == y && _lastH == h) continue;
      trans[3].tx_data[0] = y >> 8;             //Start page high
      trans[3].tx_data[1] = y & 0xFF;           //start page low
      trans[3].tx_data[2] = (y + h - 1) >> 8;   //end page high
      trans[3].tx_data[3] = (y + h - 1) & 0xFF; //end page low
      break;

    case 5:
      trans[5].tx_buffer = _pixBuf[_pixFlip];    //finally send the line data
      trans[5].length = w * 2 * 8 * h;          //Data length, in bits
      trans[5].flags = 0;                       //undo SPI_TRANS_USE_TXDATA flag
      _pixFlip = !_pixFlip;
      break;
    }
    ret = spi_device_queue_trans(_spi, &trans[i], portMAX_DELAY);
    assert(ret == ESP_OK);
    _sent[i] = true;
  }
  _lastX = x;
  _lastY = y;
  _lastW = w;
  _lastH = h;
}

void DMADrawer::draw_finish()
{
  spi_transaction_t *rtrans;
  esp_err_t ret;
  for (uint8_t i = 0; i < 6; ++i)
  {
    if (!_sent[i]) continue;
    ret = spi_device_get_trans_result(_spi, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
  }
}

