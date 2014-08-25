#include "STL.h"

STL::STL(int width, int height, SdFat *sd, SdFile *file, CRGB leds[], char *filename) {
	this->width = width;
	this->height = height;
	this->sd = sd;
	this->file = file;
	this->leds = leds;
}

void STL::draw(CHSV color) {
	// draw functions

}

void STL::load(char *filename) { 
	// load a STL into memory
	int i;
	file->open(filename, O_READ);
	// header: 80 characters - 
	for(i = 0; i<=20;i++) {
		void(read32(file));
	}
	// number of triangles: uint32_t
	this->numTriangles = read32(file);
	/*Triangle faces[this->numTriangles];

	// next is a for loop - it's normal, V1/v2/v3 / 16-bit int
	for(i = 0; i<= this->numTriangles; i++) { 
		faces[i].normal.x = readf32(file);
		faces[i].normal.y = readf32(file);
		faces[i].normal.z = readf32(file);
		for(int v =0; v<3;v++) { 
				faces[i].corner[v].x = readf32(file);
				faces[i].corner[v].y = readf32(file);
				faces[i].corner[v].z = readf32(file);
		}
		// discard next 2 bytes
		void(read16(file));
	}
*/
}

uint16_t STL::read16(SdFile* f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f->read(); // LSB
  ((uint8_t *)&result)[1] = f->read(); // MSB
  return result;
}

uint32_t STL::read32(SdFile* f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f->read(); // LSB
  ((uint8_t *)&result)[1] = f->read();
  ((uint8_t *)&result)[2] = f->read();
  ((uint8_t *)&result)[3] = f->read(); // MSB
  return result;
}

union uRealUInt { 
	uint32_t IntVersion;
	float RealVersion;
};

float STL::readf32(SdFile* f) { 
	uRealUInt result;
	((uint8_t *)&result.IntVersion)[0] = f->read();
	((uint8_t *)&result.IntVersion)[1] = f->read();
  	((uint8_t *)&result.IntVersion)[2] = f->read();
  	((uint8_t *)&result.IntVersion)[3] = f->read(); // MSB
  	return result.RealVersion;
}

int STL::XY(int x, int y) {
    y = constrain(y,0,height-1);
    x = constrain(x,0,width-1);
    return (y * width) + x;
}
