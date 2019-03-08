#include <vector>
#include <M5Stack.h>
#include <M5StackUpdater.h>     // https://github.com/tobozo/M5Stack-SD-Updater/
#include <M5TreeView.h>         // https://github.com/lovyan03/M5Stack_TreeView
#include <esp_sleep.h>

#include "src/HeaderSample.h"
#include "src/ScrollDemo.h"
#include "src/MPU9250Demo.h"
#include "src/IP5306RegEdit.h"
#include "src/UDPReciever.h"
#include "src/AD34FFTTaskDemo.h"
#include "src/ADInputDemo.h"

M5TreeView treeView;
HeaderSample header;

void drawFrame() {
  Rect16 r = treeView.clientRect;
  r.inflate(1);
  M5.Lcd.drawRect(r.x -1, r.y, r.w +2, r.h, MenuItem::frameColor[1]);
  M5.Lcd.drawRect(r.x, r.y -1, r.w, r.h +2, MenuItem::frameColor[1]);
  treeView.update(true);
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
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
     updateFromFS(SD);
     ESP.restart();
  }

  M5ButtonDrawer::width = 106;

  treeView.setTextFont(2);
  treeView.itemHeight = 20;
  treeView.clientRect.x = 2;
  treeView.clientRect.y = 18;
  treeView.clientRect.w = 196;
  treeView.clientRect.h = 200;
  treeView.itemWidth = 176;

  treeView.fontColor[0]  = 0xFFFF;
  treeView.backColor[0]  = 0x2181;
  treeView.frameColor[0] = 0x10C0;
  treeView.fontColor[1]  = 0xFFFF;
  treeView.backColor[1]  = 0x4402;
//treeView.backColor[1]  = 0x6704;
  treeView.frameColor[1] = 0xFFFF;

  treeView.useFACES       = true;
  treeView.useCardKB      = true;
  treeView.useJoyStick    = true;
  treeView.usePLUSEncoder = true;

  treeView.setItems(vmi
               { new MenuItem("ScreenShotReciever", callBackExec<UDPReciever>)
               , new MenuItem("ScrollDemo"   , callBackExec<ScrollDemo>)
               , new MenuItem("MPU9250Demo"  , callBackExec<MPU9250Demo>)
               , new MenuItem("ADInputDemo"  , callBackExec<ADInputDemo>)
               , new MenuItem("FIRE MicFFT"  , callBackExec<AD34FFTTaskDemo>)
               , new MenuItem("IP5306 Registry", 0x75, callBackRegEdit<IP5306RegEdit>)
               , new MenuItem("OTA Rollback" , callBackRollBack)
               } );
  treeView.begin();
  drawFrame();
}

void loop() {
  treeView.update();
  if (treeView.isRedraw()) {
    drawFrame();
  }
  header.draw();
}
