#include "Bitmap.h"
/*
// Usage:
	Bitmap bmp(kMatrixWidth, kMatrixHeight, &sd, &file, leds);
	bmp.Draw("path/filename.bmp",width,height);

*/
Bitmap::Bitmap(int width, int height, SdFat *sd, SdFile *file, CRGB leds[]) {
	this->width = width;
	this->height = height;
	this->sd = sd;
	this->file = file;
	this->leds = leds;
}


void Bitmap::Draw(char *filename, uint8_t x, uint8_t y) {

  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*64]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  //  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= width) || (y >= height)) return;

  // Open requested file on SD card
  file->open(filename, O_READ);

  // Parse BMP header
  if(read16(file) == 0x4D42) { // BMP signature

    void(read32(file));

    void(read32(file)); // Read & ignore creator bytes
    bmpImageoffset = read32(file); // Start of image data
    // Read DIB header

    void(read32(file));
    bmpWidth  = read32(file);
    bmpHeight = read32(file);
    if(read16(file) == 1) { // # planes -- must be '1'
      bmpDepth = read16(file); // bits per pixel
      if((bmpDepth == 24) && (read32(file) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;

        if((x+w-1) >= width)  w = width  - x;
        if((y+h-1) >= height) h = height - y;

        for (row=0; row<h; row++) { // For each scanline...
          //          tft.goTo(x, y+row);


          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
          pos = bmpImageoffset + row * rowSize;
          if(file->curPosition() != pos) { // Need seek?
            file->seekSet(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          // optimize by setting pins now
          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              file->read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to Matrix format, push to display
            CRGB color;
            color.blue = sdbuffer[buffidx++];
            color.green = sdbuffer[buffidx++];
            color.red = sdbuffer[buffidx++];
            leds[XY(x+col, y+row)] = color;
          } // end pixel
        } // end scanline
      } // end goodBmp
    }
  }

  file->close();
  if(!goodBmp) {
    Serial.print("BMP format not recognized.");
    Serial.println(filename);
  }
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t Bitmap::read16(SdFile* f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f->read(); // LSB
  ((uint8_t *)&result)[1] = f->read(); // MSB
  return result;
}

uint32_t Bitmap::read32(SdFile* f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f->read(); // LSB
  ((uint8_t *)&result)[1] = f->read();
  ((uint8_t *)&result)[2] = f->read();
  ((uint8_t *)&result)[3] = f->read(); // MSB
  return result;
}

int Bitmap::XY(int x, int y) {
    y = constrain(y,0,height-1);
    x = constrain(x,0,width-1);
    return (y * width) + x;
}
