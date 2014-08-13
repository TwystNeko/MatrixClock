#ifndef Bitmap_h
#define Bitmap_h
#include "SdFat.h"
#include "SdFatUtil.h"
#include "SmartMatrix.h"
#include "FastLED.h"

#define BM_PIXEL_X 32
#define BM_PIXEL_Y 32



class Bitmap{
public:
	SdFat *sd;
	SdFile *file;
	CRGB *leds;
	Bitmap(SdFat *sd, SdFile *file, CRGB leds[]);
	void Draw(char *filename, uint8_t x, uint8_t y);
private:
	uint16_t read16(SdFile* f);
	uint32_t read32(SdFile* f);
	int XY(int x, int y);
};
#endif