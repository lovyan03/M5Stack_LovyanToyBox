#ifndef _AD34FFTTASKDEMO_H_
#define _AD34FFTTASKDEMO_H_
#include <arduinoFFT.h>      // https://github.com/kosme/arduinoFFT

#include <MenuCallBack.h>
#undef min
#include <algorithm>

#define ADC_MIC_PIN 34
#define FFT_LEN 128
//#define SAMPLINGFREQUENCY 44100
#define SAMPLINGFREQUENCY 22050
#define SAMPLING_TIME_US (1000000UL/SAMPLINGFREQUENCY)

class AD34FFTTaskDemo : public MenuCallBack
{
public:
  AD34FFTTaskDemo() {}
  virtual void operator()(MenuItem*) {
    M5.Lcd.fillScreen(0);
    btnDrawer.setText("Back","Decay -","Decay +");
    if (setup()) {
      do {
        btnDrawer.draw();
        M5.update();
        if (M5.BtnB.wasPressed() &&  1 < decay) --decay;
        if (M5.BtnC.wasPressed() && 20 > decay) ++decay;
#ifdef _PLUSEncoder_H_
        PlusEncoder.update();
        if (PlusEncoder.isLongClick()) break;
        if (PlusEncoder.wasUp()   &&  1 < decay) --decay;
        if (PlusEncoder.wasDown() && 20 > decay) ++decay;
#endif
      } while (loop() && !M5.BtnA.wasReleased());
      close();
      M5.Lcd.fillScreen(0);
    }
  }
  bool setup()
  {
    pinMode(ADC_MIC_PIN, INPUT);
    pinMode(25, OUTPUT);
    dacWrite(25, 0);
    isRunningFFT = true;
    frameMain = 0;
    frameFFT = 0;
    xTaskCreatePinnedToCore(taskFftLoop, "taskfft", 4096, this, 1, NULL, 0);
    startMsec = millis();
    return true;
  }
  void close() {
    isRunningFFT = false;
  }
  bool loop()
  {
    int x, y, py;
    ++frameMain;
    while (frameMain != frameFFT); // frame sync wait
    for (int i = 1; i < FFT_LEN / 2; ++i) {
      if (peakFft[i] < fftdata[i])  peakFft[i] = fftdata[i];
    }
    if (0 == (frameMain % 2)) {
      for (int i = 1; i < FFT_LEN / 2; ++i)
      {
        x = 128 + i * 3 - 1;
        y = peakFft[i];
        py = pyFft[i];
        if (py > y) { 
          y = (y + py * (decay - 1)) / decay; // Slow decay
          M5.Lcd.fillRect(x, 222-py, 2, py-y, 0);
        }
        M5.Lcd.fillRect(x, 222-y, 2, y+1, 0x001f-(y>>3) + ((y>>2)<<5) + ((y>>3)<<11));
        pyFft[i] = y;
        peakFft[i] = 0;
      }
    } else if (1 == (frameMain % 2)) {
      for (int i = 0; i < FFT_LEN; ++i) {
        if (i != FFT_LEN - 1) { 
          if (pyWav[i+1] < pyWav[i])      M5.Lcd.drawFastVLine(i+1, pyWav[i+1], pyWav[i] - pyWav[i+1]+1,0);
          else if (pyWav[i+1] > pyWav[i]) M5.Lcd.drawFastVLine(i+1, pyWav[i], pyWav[i+1] - pyWav[i]+1,0);
          else                            M5.Lcd.drawPixel(i+1, pyWav[i], 0);
        }
        y = 8 + rawdata[i] / 19;
        if (i != 0) { 
          if (py < y)      M5.Lcd.drawFastVLine(i, py, y - py+1, 0xffff);
          else if (py > y) M5.Lcd.drawFastVLine(i, y, py - y +1, 0xffff);
          else             M5.Lcd.drawPixel(i, y, 0xffff);
        }
        py = y;
        pyWav[i] = y;
      }
    }
    uint32_t m = millis() - startMsec;
    if (m != 0) {
      M5.Lcd.setTextColor(0xFFFF,0);
      M5.Lcd.setCursor(0,0);
      M5.Lcd.printf("FPS%3d    decay %2d", (int)((double)frameMain * 1000 / m), decay);
    }
    return true;
  }
private:
  unsigned int decay = 5;
  uint8_t pyWav[FFT_LEN] = {0};
  uint8_t pyFft[FFT_LEN / 2] = {0};
  uint8_t peakFft[FFT_LEN / 2] = {0};
  volatile uint8_t fftdata[FFT_LEN / 2];
  volatile uint16_t rawdata[FFT_LEN] = {0};
  volatile bool isRunningFFT;
  volatile uint32_t frameFFT = 0;
  uint32_t startMsec;
  uint32_t frameMain;

  static void taskFftLoop(void* arg) {
    AD34FFTTaskDemo* Me = (AD34FFTTaskDemo*)arg;
    Me->frameFFT = 0;
    uint32_t nextTime = 0;
    double AdcMeanValue = 0;
    int x, y, n;
    arduinoFFT FFT = arduinoFFT();
    double adBuf[FFT_LEN];
    double vImag[FFT_LEN] = {0};
    while (Me->isRunningFFT) {
      FFT.Windowing(adBuf, FFT_LEN, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(adBuf, vImag, FFT_LEN, FFT_FORWARD);
      FFT.ComplexToMagnitude(adBuf, vImag, FFT_LEN);
      for (n = 0; n < FFT_LEN / 2; n++) {
        Me->fftdata[n] = map(std::min(256.0, adBuf[n]/32), 0, 256, 0, 212);
      }
      delay(1);
      ++Me->frameFFT;
      while (Me->isRunningFFT && Me->frameMain != Me->frameFFT); // frame sync wait
      for (n = 0; n < FFT_LEN; n++) {
        vImag[n] = 0;
        double v = analogRead(ADC_MIC_PIN);
        Me->rawdata[n] = v;
        AdcMeanValue += (v - AdcMeanValue) * 0.001;
        adBuf[n] = v - AdcMeanValue;
        while (micros() < nextTime);
        nextTime = micros() + SAMPLING_TIME_US;
      }
    }
    vTaskDelete(xTaskGetCurrentTaskHandle());
  }
};
#endif
