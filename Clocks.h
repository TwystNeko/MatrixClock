#ifndef Clocks_h
#define Clocks_h
// Clock drawing functions - Digital and Analog

#define ipart_(X) ((int)(X))
#define round_(X) ((int)((double)(X))+0.5)
#define fpart_(X) (((double)(X)) - (double)ipart_(X))
#define rfpart_(X) (1.0 - fpart_(X))
#define swap_(a,b) do { __typeof__(a) tmp; tmp = a; a = b; b = tmp; } while(0)

class Clock
{
public:
	CHSV hColor = CHSV(0,255,255);
	CHSV mColor = CHSV(60,255,255);
	CHSV sColor = CHSV(120,255,255);
	bool useWuLine = false;
	int width;
	int height;
	CRGB *leds;
	Clock(int width, int height, CRGB leds[]);
	void analogClock(int x, int y, int radius);
private:
	void setHand(int iVal, int iMax, int iRadius, CHSV iColor, int x, int y);
	void wuPlot(uint8_t x, uint8_t y, float fade, CHSV color);
	void wuLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, CHSV color);
	void breLine(int x0, int y0, int x1, int y1, CHSV color);
	//void Line(int x0, int y0, int x1, int y1, CHSV c);
	//void drawPixel(int x, int y, CHSV c);
	int XY(int x, int y);
	/* data */
};

#endif