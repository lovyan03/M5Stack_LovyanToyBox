#ifndef _I2CREGEDIT_H_
#define _I2CREGEDIT_H_

class I2CRegEdit
{
public:
  uint8_t addr = 0x75;
  uint8_t regMax = 0x90;

  void operator()(MenuItem* mi) {
    menuItem = mi;
    treeView = ((M5TreeView*)(mi->topItem()));
    M5.Lcd.fillScreen(0);
    btnDrawer.setText("Back/Prev","Ok","Next");
    if (setup()) {
      while (loop());
      close();
      M5.Lcd.fillScreen(MenuItem::backgroundColor);
    }
  }

  bool setup() 
  {
    M5.Lcd.setTextColor(0xFFFF);
    for (int i = 1; i < 16; ++i) {
      M5.Lcd.drawFastHLine(0, i, M5.Lcd.width(), i << 1);
    }
    M5.Lcd.drawString(getTitle(), 10, 0, 2);

    M5.Lcd.setTextFont(1);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(0xFF00, 0);

    for (byte i = 0; i < regMax/16; ++i) {
      M5.Lcd.setCursor(2, 32 + i*12);
      M5.Lcd.printf("%01X", i);
    }
    for (byte i = 0; i < 0x10; ++i) {
      M5.Lcd.setCursor(26+i*19, 20);
      M5.Lcd.printf("%01X", i);
    }
    return true;
  }

  bool loop()
  {
    btnDrawer.draw();

    uint8_t focusRegOld = _focusReg;

    eInput cmd = getInput();
    switch (_mode) {
    case eMode::SELECT:

      switch (cmd) {
      case eInput::LEFT:  _focusReg = (_focusReg + regMax - 1)  % regMax; break;
      case eInput::RIGHT: _focusReg = ++_focusReg               % regMax; break;
      case eInput::UP:    _focusReg = (_focusReg + regMax - 16) % regMax; break;
      case eInput::DOWN:  _focusReg = (_focusReg + 16)          % regMax; break;
      case eInput::ENTER: _mode = eMode::BITEDIT; break;
      case eInput::EXIT: return false;
      }
      break;

    case eMode::BITEDIT:
      switch (cmd) {
      case eInput::LEFT:  _focusBit =  ++_focusBit      % 8; break;
      case eInput::RIGHT: _focusBit = (--_focusBit + 8) % 8; break;
      case eInput::ENTER: toggleREG(_focusReg, 1 << _focusBit); break;
      case eInput::EXIT:  _mode = eMode::SELECT; break;
      }
      break;
    }

    M5.Lcd.setTextFont(1);
    M5.Lcd.setTextSize(1);
    for (byte row = 0; row < regMax/16; ++row) {
      for (byte col = 0; col < 0x10; ++col) {
        int reg = col + row * 16;
        int x = 20 + col * 19;
        int y = 32 + row * 12;
        bool focus { _focusReg == reg };
        uint16_t color = focus ? 0x083F : 0;
        M5.Lcd.setCursor(x, y);

        if (reg == _focusReg || reg == focusRegOld) {
          M5.Lcd.drawRect(x-3, y-2, 17, 11, focus && _mode == eMode::BITEDIT ? 0xFFFF : color);
          M5.Lcd.fillRect(x-2, y-1, 15,  9, color);
        }
        Wire.beginTransmission(addr);
        Wire.write(reg);
        if (Wire.endTransmission(false) == 0
         && Wire.requestFrom(addr, (uint8_t)1)) {
          uint8_t dat = Wire.read();
          M5.Lcd.setTextColor((dat ? 0xFFFF : 0xF9E7), color);
          M5.Lcd.printf("%02X", dat);
        } else {
          M5.Lcd.setTextColor(0xF9E7, color);
          M5.Lcd.print("--");
        }
      }
    }

    uint8_t value = getREG(_focusReg);
    for ( int i = 0; i < 8; ++i ) {
      M5.Lcd.setTextColor(0xFFFF, (_focusBit == i && _mode == eMode::BITEDIT) ? 0x083F : 0);
      M5.Lcd.drawString(value & (1 << i) ? "1":"0", 150 - i * 10, 190, 2);
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

    return true;
  }

  void close()
  {
  }

protected:
  M5ButtonDrawer btnDrawer;
  M5TreeView* treeView;
  MenuItem* menuItem;

  virtual String getTitle() {
    return "I2C 0x" + String(addr,HEX) + " REGISTRY ";
  }

  uint8_t _focusReg = 0;
  uint8_t _focusBit = 0;
  enum eMode
  { SELECT
  , BITEDIT
  };
  eMode _mode { SELECT };

  void toggleREG(uint8_t reg, uint8_t bit)
  {
      uint8_t data = getREG(reg);
      setREG(reg, (data & bit) ? (data & ~bit) : (data | bit));
  }

  uint8_t getREG(uint8_t reg)
  {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) == 0
    &&  Wire.requestFrom(addr, (uint8_t)1)) {
      return Wire.read();
    }
    return 0;
  }

  void setREG(uint8_t reg, uint8_t data)
  {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
  }

  enum eInput
  { NONE
  , UP
  , DOWN
  , LEFT
  , RIGHT
  , ENTER
  , EXIT
  };
  bool _flgFACESKB = false;
  uint32_t _repeat = 0, _msec = 0, _msecLast = 0;
  eInput getInput()
  {
    bool btnALong = M5.BtnA.pressedFor(treeView->msecHold);
    _msec = millis();
    M5.update();
    eInput res = eInput::NONE;
    bool canRepeat = _repeat == 0 || (_msec - _msecLast + _repeat) >= (1 < _repeat ? treeView->msecRepeat : treeView->msecHold);
    bool press = M5.BtnA.isPressed() || M5.BtnB.isPressed() || M5.BtnC.isPressed();
    if (canRepeat) {
      if (M5.BtnA.wasReleased() && !btnALong) { res = eInput::EXIT; }
      else if (M5.BtnC.isPressed() )  { ++_repeat; res = eInput::RIGHT;  }
      else if (M5.BtnB.wasReleased()) { res = eInput::ENTER; }
      else if (btnALong) { ++_repeat; res = eInput::LEFT; }
    }
    if (res == eInput::NONE
     && treeView->useFACES && Wire.requestFrom(0x08, 1)) {
      while (Wire.available()){
        char key = Wire.read();
        if (key == 0) { _flgFACESKB = true; }
        else {
          if (key == 0xff) { _flgFACESKB = false; }
          else press = true;
          if (_flgFACESKB) {
            //
          } else {
            if (key != 0xff && canRepeat) {
              ++_repeat;
  //          if (!(key & 0x80)) { return false; }  // FACES GameBoy Start: Finish.
  //          if (!(key & 0x40)) { switchTable(); } // FACES GameBoy Select: Panel Switch.
              if (!(key & 0x10)) { res = eInput::ENTER; }
              if (!(key & 0x20)) { res = eInput::EXIT;  }
              if (0 == (key & 0x08)) { res = eInput::RIGHT; }
              if (0 == (key & 0x04)) { res = eInput::LEFT;  }
              if (0 == (key & 0x02)) { res = eInput::DOWN;  }
              if (0 == (key & 0x01)) { res = eInput::UP;    }
            }
          }
        }
      }
    }
    if (!press) {
      _repeat = 0;
    }
    if (res != eInput::NONE) {
      _msecLast = millis();
    }
    return res;
  }
};

#endif
