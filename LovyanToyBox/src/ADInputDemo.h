#ifndef _ADINPUTDEMO_H_
#define _ADINPUTDEMO_H_

#include <MenuCallBack.h>

class ADInputDemo : public MenuCallBack
{
public:
  ADInputDemo() {}
  bool setup()
  {
    pinMode(35,INPUT);
    pinMode(36,INPUT);
    dacWrite(25, 0); // anti speaker noise.
    _x10 = 0;
    for (int i = 0 ; i < 10; ++i) {
      M5.Lcd.drawLine(0,210 - i * 20, 320, 210 - i * 20, 0x4208);
    }
    for (int i = 0 ; i < 16; ++i) {
      M5.Lcd.drawLine(10 + i * 20, 20, 10 + i * 20, 220, 0x4208);
    }
    for (int i = 0 ; i < 11; ++i) {
      M5.Lcd.drawLine(0,220 - i * 20, 320, 220 - i * 20, 0x8410);
    }
    for (int i = 0 ; i < 16; ++i) {
      M5.Lcd.drawLine(i * 20, 20, i * 20, 220, 0x8410);
    }
    return true;
  }

  bool loop()
  {
    _ad35sum += analogRead(35);
    _ad36sum += analogRead(36);
    if (++_sumCount % 8 == 0) {
      uint16_t ad35 = _ad35sum / _sumCount;
      uint16_t ad36 = _ad36sum / _sumCount;
      int y_35 = 220 - ad35 / 20;
      int y_36 = 220 - ad36 / 20;
      M5.Lcd.drawPixel(_x10 / 10, y_35, 0xffe0);
      M5.Lcd.drawPixel(_x10 / 10, y_36, 0x001f);
      M5.Lcd.setTextSize(1);
      M5.Lcd.setTextColor(0xffff, 0);
      M5.Lcd.setCursor(0,0);
      M5.Lcd.printf("AD35 :%04d / AD36 :%04d", ad35, ad36);
      _x10 = (_x10 + 1) % (M5.Lcd.width() * 10);
      _sumCount = 0;
      _ad35sum = 0;
      _ad36sum = 0;
    }
    return true;
  }

private:
  uint16_t _x10 = 0;
  uint32_t _ad35sum = 0;
  uint32_t _ad36sum = 0;
  uint8_t _sumCount = 0;
};

#endif
