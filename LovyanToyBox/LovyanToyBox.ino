#include <vector>
#include <M5Stack.h>
#include <M5StackUpdater.h>     // https://github.com/tobozo/M5Stack-SD-Updater/
#include <M5TreeView.h>         // https://github.com/lovyan03/M5Stack_TreeView
#include <MenuItemToggle.h>
#include <esp_sleep.h>

#include "src/AD34FFTTaskDemo.h"
#include "src/ADInputDemo.h"
#include "src/ULPBackLight.h"
#include "src/Header.h"
#include "src/IP5306RegEdit.h"
#include "src/MPU9250Demo.h"
#include "src/ScrollDemo.h"
#include "src/TCPReceiver.h"

M5TreeView treeView;

constexpr char* preferName     ( "LovyanLauncher" );
constexpr char* preferKeyStyle ( "TVStyle" );
void drawFrame() {
  Rect16 r = treeView.clientRect;
  r.inflate(1);
  M5.Lcd.drawRect(r.x -1, r.y, r.w +2, r.h, MenuItem::frameColor[1]);
  M5.Lcd.drawRect(r.x, r.y -1, r.w, r.h +2, MenuItem::frameColor[1]);
  treeView.update(true);
}

void setStyle(int tag)
{
  switch (tag) {
  default: return;
  case 0:
    M5ButtonDrawer::height = 14;
    M5ButtonDrawer::setTextFont(1);
    treeView.setTextFont(1);
    treeView.itemHeight = 18;
    break;

  case 1:
    M5ButtonDrawer::height = 18;
    M5ButtonDrawer::setTextFont(2);
    treeView.setTextFont(2);
    treeView.itemHeight = 20;
    break;

  case 2:
    M5ButtonDrawer::height = 18;
    M5ButtonDrawer::setTextFont(2);
    treeView.setFreeFont(&FreeSans9pt7b);
    treeView.itemHeight = 24;
    break;
  }
  treeView.updateDest();
  M5.Lcd.fillRect(0, 218, M5.Lcd.width(), 22, 0);
}

void callBackRollBack(MenuItem* sender)
{
  if( Update.canRollBack() )  {
    Update.rollBack();
    ESP.restart();
  }
}

template <class T>
void callBackExec(MenuItem* sender)
{
  T menucallback;
  menucallback(sender);
}

template <class T>
void callBackRegEdit(MenuItem* sender)
{
  T menucallback;
  menucallback.addr = sender->tag;
  menucallback(sender);
}

//======================================================================//
typedef std::vector<MenuItem*> vmi;

void setup() {
  M5.begin();
#ifdef ARDUINO_ODROID_ESP32
  M5.battery.begin();
#else
  M5.Speaker.begin();
  M5.Speaker.mute();
  Wire.begin();
#endif
  if(digitalRead(BUTTON_A_PIN) == 0) {
     Serial.println("Will Load menu binary");
     updateFromFS(SD);
     ESP.restart();
  }

  M5ButtonDrawer::width = 106;
  treeView.clientRect.x = 2;
  treeView.clientRect.y = 16;
  treeView.clientRect.w = 196;
  treeView.clientRect.h = 200;
  treeView.itemWidth = 176;

  treeView.fontColor[0]  = 0xFFFF;
  treeView.backColor[0]  = 0x2181;
  treeView.frameColor[0] = 0x10C0;
  treeView.fontColor[1]  = 0xFFFF;
  treeView.backColor[1]  = 0x4602;
  treeView.frameColor[1] = 0xFFFF;

  treeView.useFACES       = true;
  treeView.useCardKB      = true;
  treeView.useJoyStick    = true;
  treeView.usePLUSEncoder = true;


// restore setting
  Preferences p;
  p.begin(preferName, true);
  setStyle(p.getUChar(preferKeyStyle, 1));
  p.end();

  treeView.setItems(vmi
               { new MenuItem("ScreenShotReceiver"    , 0, callBackExec<TCPReceiver>)
               , new MenuItem("ScreenShotReceiver(AP)", 1, callBackExec<TCPReceiver>)
               , new MenuItem("ScrollDemo"   , callBackExec<ScrollDemo>)
               , new MenuItem("MPU9250Demo"  , callBackExec<MPU9250Demo>)
               , new MenuItem("ADInputDemo"  , callBackExec<ADInputDemo>)
               , new MenuItem("FIRE MicFFT"  , callBackExec<AD34FFTTaskDemo>)
               , new MenuItem("ULP BackLight Demo", callBackExec<ULPBackLight>)
               , new MenuItem("IP5306 Registry", 0x75, callBackRegEdit<IP5306RegEdit>)
               , new MenuItem("OTA Rollback", vmi
                   { new MenuItem("Rollback Execute", callBackRollBack)
                   } )
               } );
  treeView.begin();
  drawFrame();
}

uint8_t loopcnt = 0xF;
long lastctrl = millis();
void loop() {
  if (NULL != treeView.update()) {
    lastctrl = millis();
  }
  if (treeView.isRedraw()) {
    drawFrame();
    loopcnt = 0xF;
  }
  if (0 == (++loopcnt & 0xF)) {
    Header.draw();
#ifndef ARDUINO_ODROID_ESP32
    if ( 600000 < millis() - lastctrl ) {
      Serial.println( "goto sleep" );
      M5.Lcd.setBrightness(0);
      M5.Lcd.sleep();
      esp_deep_sleep_start();
    }
#endif
  }
}
