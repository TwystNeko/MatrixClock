#include "STL.h"

STL::STL(int width, int height, SdFat *sd, SdFile *file, CRGB leds[], char *filename) {
	this->width = width;
	this->height = height;
	this->sd = sd;
	this->file = file;
	this->leds = leds;
}

void STL::scale(float scX, float scY, float scZ) { 
	// scale
}

void STL::rotate(float angX, float angY) {
	// rotate
    int i,j;
    int a,b,c;
    float cx=cos(angX);
    float sx=sin(angX);
    float cy=cos(angY);
    float sy=sin(angY);
    float m00,m01,m02,m10,m11,m12,m20,m21,m22;
    m00=cy;  
    m01=0;  
    m02=-sy;

    m10=sx*sy;
    m11=cx;
    m12=sx*cy;

    m20=cx*sy;
    m21=-sx;
    m22=cx*cy;
    for(i=0; i<this->numTriangles; i++) {
    	// for each face... 
    	for(j=0;j<4;j++) { 
    		// transform each vertex, including the normal

    	}

    }
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
	float faces[numTriangles][4][3];

	// next is a for loop - it's normal, V1/v2/v3 / 16-bit int
	for(i = 0; i<= this->numTriangles; i++) { 
		for(int v =0; v<4;v++) { 
			for(int c = 0; c<3;c++) { 
				faces[i][v][c] = readf32(file);
			}
		}
		// discard next 2 bytes
		void(read16(file));
	}

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

float STL::readf32(SdFile* f) { 
	float result;
	((uint8_t *)&result)[0] = f->read();
	((uint8_t *)&result)[1] = f->read();
  	((uint8_t *)&result)[2] = f->read();
  	((uint8_t *)&result)[3] = f->read(); // MSB
  	return result;
}

int STL::XY(int x, int y) {
    y = constrain(y,0,height-1);
    x = constrain(x,0,width-1);
    return (y * width) + x;
}
