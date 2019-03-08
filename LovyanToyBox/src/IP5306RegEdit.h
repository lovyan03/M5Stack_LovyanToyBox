#ifndef _IP5306REGEDIT_H_
#define _IP5306REGEDIT_H_

#include "I2CRegEdit.h"

class IP5306RegEdit : public I2CRegEdit
{
public:
  bool loop()
  {
    I2CRegEdit::loop();

    M5.Lcd.setTextColor(0xFFFF, 0);
    M5.Lcd.setCursor(0, 160);
    M5.Lcd.print("isCharging          :");
    M5.Lcd.println((getREG(0x70) & 0x08) ? "ON" : "--");
    if (_focusReg == 0x00) {
      M5.Lcd.print("Boost enable        :");
      M5.Lcd.println((getREG(0x00) & 0x20) ? "ON" : "--");
      M5.Lcd.print("Charge enable       :");
      M5.Lcd.println((getREG(0x00) & 0x10) ? "ON" : "--");
      M5.Lcd.print("Insert auto power on:");
      M5.Lcd.println((getREG(0x00) & 0x04) ? "ON" : "--");
      M5.Lcd.print("Boost normally open :");
      M5.Lcd.println((getREG(0x00) & 0x02) ? "ON" : "--");
      M5.Lcd.print("Key off enable      :");
      M5.Lcd.println((getREG(0x00) & 0x01) ? "ON" : "--");
    }

/*
    if (M5.BtnB.wasPressed()) { // Battery Charge on of toggle
      toggleREG(0, 0x10);
    }

    if (M5.BtnC.wasPressed()) { // Battery Boost on of toggle
      //toggleREG(0, 0x20);
      toggleREG(0, 0x02);
    }
*/
    delay(1);
    return true;
  }
protected:
  
};

#endif
