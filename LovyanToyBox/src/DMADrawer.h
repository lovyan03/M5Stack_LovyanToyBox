#ifndef _DMADRAWER_H_
#define _DMADRAWER_H_

#include <stdint.h>
#include <stdio.h>

struct DMADrawer
{
private:
  uint32_t _buflen;
  uint8_t _pixFlip = 0;
  uint16_t* _pixBuf[2];
  bool _sent[6];
  uint16_t _lastX, _lastY, _lastW, _lastH;

public:
  DMADrawer() {}

  bool setup(uint32_t buflen);
  void close();
  uint16_t* getNextBuffer();
  void draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void draw_finish();
};

#endif
