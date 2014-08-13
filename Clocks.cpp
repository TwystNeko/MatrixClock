#include "SmartMatrix.h"
#include "FastLED.h"
#include "Clocks.h"
#include "Time.h"
#include "math.h"

/*
Usage:
  Clock clock(width, height, leds);
  clock.useWuLine = false; // Wu lines, or Bresenham lines
  clock.hColor = CHSV(60,255,255); // Hand colors - hColor, mColor, sColor
  clock.analogClock(x,y,radius);

*/
  
Clock::Clock(int width, int height,CRGB leds[]) { 
  this->width = width;
  this->height = height;
  this->leds = leds;
}

void Clock::analogClock(int x, int y, int radius) {

    setHand(hour(), 12, radius - 2, hColor, x, y);
    setHand(minute(), 60, radius -1, mColor, x, y);
    setHand(second(), 60, radius, sColor, x, y);
}

int Clock::XY(int x, int y) { 
    y = constrain(y,0,height-1);
    x = constrain(x,0,width-1);
    return (y * width) + x;
}

void Clock::setHand(int iVal, int iMax, int iRadius, CHSV iColor, int x, int y) {
  double dAngle = ((iVal * 2 * M_PI) / iMax) -1.57;
  double dPosX =x + (iRadius * cos(dAngle));
  double dPosY =y + (iRadius * sin(dAngle));
  if(useWuLine) {
    wuLine(x,y, constrain(round(dPosX),1,32), constrain(round(dPosY),1,32), iColor);
  } else { 
    breLine(x,y, constrain(round(dPosX),1,32), constrain(round(dPosY),1,32), iColor);
  }
}

void Clock::breLine(int x0, int y0, int x1, int y1, CHSV color) { 
  int dx = abs(x1 - x0), sx = x0<x1 ? 1: -1;
  int dy = abs(y1 - y0), sy = y0<y1 ? 1: -1;
  int err = (dx>dy ? dx : -dy)/2, e2;
  for(;;) { 
    leds[XY(x0,y0)] = color;
    if(x0 == x1 && y0 == y1) break;
    e2 = err;
    if(e2 > -dx) { err -= dy; x0 +=sx; }
    if(e2 < dy) { err += dx; y0 +=sy;}
  } 
}

void Clock::wuPlot(uint8_t x, uint8_t y, float fade, CHSV color) { 
  if(fade > 1.0) fade = 1.0;
  color.value = color.value * fade;
  leds[XY(x,y)] = color;
}

void Clock::wuLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, CHSV color) { 
 
  bool steep = abs(y1 - y0) > abs(x1 -x0);
  if(steep) { 
    swap_(x0,y0);
    swap_(x1,y1);
  }
  if(x0 > x1) { 
    swap_(x0,x1);
    swap_(y0,y1);
  }

  double dx = (double)x1 - (double)x0;
  double dy = (double)y1 - (double)y0;
  double gradient = dy / dx;

  // first endpoint
  double xend = round_(x0);
  double yend = y0 + gradient * (xend - x0);
  double xgap = rfpart_(x0 + 0.5);
  double xpx11 = xend;
  double ypx11 = ipart_(yend);
  if(steep) { 
    wuPlot(ypx11, xpx11, rfpart_(yend) * xgap, color);
    wuPlot(ypx11+1, xpx11, fpart_(yend) * xgap, color);
  } else { 
    wuPlot(xpx11, ypx11, rfpart_(yend) * xgap, color);
    wuPlot(xpx11, ypx11+1, rfpart_(yend) * xgap, color);
  }
  double intery = yend +gradient;

  // second endpoint
  xend = round_(x1);
  yend = y1 + gradient * (xend - x1);
  xgap = fpart_(x1 + 0.5);
  double xpx12 = xend;
  double ypx12 = ipart_(yend);
  if(steep) { 
    wuPlot(ypx12, xpx12, rfpart_(yend) * xgap, color);
    wuPlot(ypx12+1, xpx12, fpart_(yend) * xgap, color);
  } else { 
    wuPlot(xpx12, ypx12, rfpart_(yend) * xgap, color);
    wuPlot(xpx12, ypx12+1, rfpart_(yend) * xgap, color);
  }

  // main loop
  for (int x = xpx11+1; x < xpx12; x++)
  {
    /* code */
    if(steep) { 
      wuPlot(ipart_(intery), x, rfpart_(intery), color);
      wuPlot(ipart_(intery)+1, x, fpart_(intery), color);
    } else { 
      wuPlot(x, ipart_(intery), rfpart_(intery), color);
      wuPlot(x, ipart_(intery)+1,fpart_(intery), color);
    }
    intery+= gradient;
  }
}