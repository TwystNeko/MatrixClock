#ifndef STL_h
#define STL_h

#include "SdFat.h"
#include "SdFatUtil.h"
#include "SmartMatrix.h"
#include "FastLED.h"

struct Triangle{
	Point3d corner[3];
	Point3d normal;
};

class STL{

public:
	SdFat *sd;
	SdFile *file;
	CRGB *leds;
	int width;
	int height;
	Triangle faces;
	uint32_t numTriangles;
	STL(int width, int height, SdFat *sd, SdFile *file, CRGB leds[], char *filename);
	void draw(CHSV color);

private:
	uint16_t read16(SdFile* f);
	uint32_t read32(SdFile* f);
	float readf32(SdFile* f);
	int XY(int x, int y);
	void load(char *filename);
};


#endif