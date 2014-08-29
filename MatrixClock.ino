// This is a collection of snippets and 
#include "SmartMatrix.h"
#include "FastLED.h"
#include "Time.h"
#include "IRremote.h"
#include "SdFat.h"
#include "SdFatUtil.h"
#include "IRCodes.h"
#include "math.h"
#include "Bitmap.h"
#include "Clocks.h"
#include "Funcs3d.h"
#include "icosahedron.h"

SdFat sd;
SdFile file;

const byte numParticles = 30;



#define kMatrixWidth  32
#define kMatrixHeight 32
#define XCENTER 15.5
#define YCENTER 15.5

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define DEFAULT_BRIGHTNESS 255

#define IR_RECV_CS 16
#define SD_CS 15

#define MODE_TIMEOUT 30

CRGB leds[kMatrixWidth * kMatrixHeight];

Bitmap bmp(kMatrixWidth, kMatrixHeight, &sd, &file, leds);

Clock clock(kMatrixWidth,kMatrixHeight, leds);

const rgb24 COLOR_BLACK = { 0,0,0 };
const rgb24 COLOR_WHITE = { 255,255,255};

int8_t cubeHue = 0;

int8_t currentMode = 0;
uint32_t nextMode = millis() + (MODE_TIMEOUT * 1000);
const int8_t maxMode = 3;
bool initMode = true;
bool autoMode = true;
struct timer { 
  uint32_t wait;
  uint32_t lastMillis;
  uint32_t count;
  int8_t delta;
  byte max;
  byte min;
};

timer multiTimer[5];

IRrecv irReceiver(IR_RECV_CS);

#define MESH_VERTICES 12
#define MESH_FACES 20
#define MESH_FOV 45

Object3D Icosa(MESH_VERTICES, MESH_FACES);

bool MESH_WIREFRAME = false;

float AngX = 20.0;
float AngY = 10.0;

bool isOff = false;
bool clockOption = true;
bool showClock = true;

uint32_t nextFrame = 0;

int8_t timers = sizeof(multiTimer) / sizeof(multiTimer[0]);

uint8_t noise[kMatrixWidth][kMatrixHeight];
static uint16_t x;
static uint16_t y;
static uint16_t z;
uint16_t speed = 20; 
uint16_t scale = 31;

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

int XY(int x, int y) {
    y = constrain(y,0,kMatrixHeight-1);
    x = constrain(x,0,kMatrixWidth-1);
    return (y * kMatrixWidth) + x;
}

void setup() {
  LEDS.addLeds<SmartMatrix>(leds,NUM_LEDS);
        LEDS.setBrightness(DEFAULT_BRIGHTNESS);
        pSmartMatrix->setColorCorrection(cc24);
        setSyncProvider(getTeensy3Time);
        pSmartMatrix->fillScreen(COLOR_BLACK);
        LEDS.show();

        irReceiver.enableIRIn();

        // Set up oscillators
        for(int i = 0; i<timers; i++) multiTimer[i].delta = 1;
        multiTimer[0].wait = 42;
        multiTimer[0].max = kMatrixWidth - 1;
        multiTimer[0].min = 0;
        multiTimer[0].count = 0;

        multiTimer[1].wait = 55;
        multiTimer[1].max = kMatrixHeight - 1;
        multiTimer[1].min = 0;
        multiTimer[1].count = 0;

        multiTimer[2].wait = 3;
        multiTimer[2].max = 255;
        multiTimer[2].min = 0;
        multiTimer[2].count = 0;

        multiTimer[3].wait = 71;
        multiTimer[3].max = kMatrixWidth - 1;
        multiTimer[3].min = 0;
        multiTimer[3].count = 0;

        multiTimer[4].wait = 89;
        multiTimer[4].max = kMatrixHeight - 1;
        multiTimer[4].min = 0;
        multiTimer[4].count = 0;
        Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!sd.begin(SD_CS, SPI_FULL_SPEED)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("SD OK!");
  Icosa.loadMesh(icosahedron_v, icosahedron_f);

}

void transformMesh() { 
  Icosa.Rotate(AngX,AngY,0);
  Icosa.Translate(0,0,-30);
  Icosa.Scale(1);

  Icosa.ApplyTransforms();
  Icosa.sortDepthMap();
  AngX +=.05;
  AngY +=.05;

}

void drawMesh(int Hue) { 
  int ax, ay,bx,by,cy,cx,V,baseHue;
  baseHue = Hue;
  for(int i=0; i<MESH_FACES; i++) {
    ax = Icosa.Render(XCENTER,MESH_FOV,i,'a');
    ay = Icosa.Render(YCENTER,MESH_FOV,i,'b');

    bx = Icosa.Render(XCENTER,MESH_FOV,i,'c');
    by = Icosa.Render(YCENTER,MESH_FOV,i,'d');

    cx = Icosa.Render(XCENTER,MESH_FOV,i,'e');
    cy = Icosa.Render(YCENTER,MESH_FOV,i,'f');

    if(MESH_WIREFRAME) { 
      V = map(Icosa.depthMap[i].depth,Icosa.minDepth,Icosa.maxDepth,100,255);
        pSmartMatrix->drawTriangle(ax,ay,bx,by,cx,cy, CRGB(CHSV(baseHue,255,V)));
        baseHue +=10;
    } else { 
      V = map(Icosa.depthMap[i].depth,Icosa.minDepth,Icosa.maxDepth,-100,255); // larger "range" as back faces won't be drawn
      V = constrain(V,0,255);
      pSmartMatrix->fillTriangle(ax,ay,bx,by,cx,cy, CRGB(CHSV(baseHue,255,V)));
      baseHue +=10;
    }      
  }
}

void drawIcosahedron() { 
  pSmartMatrix->fillScreen(CRGB(0,0,0));
  transformMesh();
  drawMesh(cubeHue);
  cubeHue++;
  nextFrame = millis() + 10;
}

void UpdateTimers() 
{
  uint32_t now = millis();
  for(int i = 0; i < timers; i++)
  {
    while(now - multiTimer[i].lastMillis >= multiTimer[i].wait)
    {
      multiTimer[i].lastMillis += multiTimer[i].wait;
      multiTimer[i].count += multiTimer[i].delta;
      if((multiTimer[i].count == multiTimer[i].max) || (multiTimer[i].count == multiTimer[i].min))
      {
        multiTimer[i].delta = -multiTimer[i].delta;
      }
    }
  } 
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  for(int i = 0; i < kMatrixWidth; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < kMatrixHeight; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset,y + joffset,z);
    }
  }
  z += speed;
}


void Spiral(int x, int y, int r, byte dimm) {
    for (int d = r; d >= 0; d--) {
        for (int i = x - d; i <= x + d; i++) {
            leds[XY(i, y - d)] += leds[XY(i + 1, y - d)];
            leds[XY(i, y - d)].nscale8(dimm);
        }
        for (int i = y - d; i <= y + d; i++) {
            leds[XY(x + d, i)] += leds[XY(x + d, i + 1)];
            leds[XY(x + d, i)].nscale8(dimm);
        }
        for (int i = x + d; i >= x - d; i--) {
            leds[XY(i, y + d)] += leds[XY(i - 1, y + d)];
            leds[XY(i, y + d)].nscale8(dimm);
        }
        for (int i = y + d; i >= y - d; i--) {
            leds[XY(x - d, i)] += leds[XY(x - d, i - 1)];
            leds[XY(x - d, i)].nscale8(dimm);
        }
    }
}


void DimAll(byte value)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i].nscale8(value);
    }
}

// IR Functions 
// =====================================

unsigned long handleInput() {
    unsigned long input = 0;

    decode_results results;

    results.value = 0;

    // Attempt to read an IR code ?
    if (irReceiver.decode(&results)) {
        input = results.value;

        // Prepare to receive the next IR code
        irReceiver.resume();
    }
    if(input == IRCODE_UP) { 
      currentMode++; 
      if(currentMode > maxMode ) { currentMode = 0; }
      nextFrame = 0;
      nextMode = millis() + (MODE_TIMEOUT * 1000);
      initMode = true;
    }
    if(input == IRCODE_DOWN) { 
      currentMode--; 
      if(currentMode < 0 ) { currentMode = maxMode; }
      nextFrame = 0;
      nextMode = millis() + (MODE_TIMEOUT * 1000);
      initMode = true;
    }

    if (input == IRCODE_SEL) { 
      clockOption = !clockOption;
    }
    if(input == IRCODE_A) { 
      autoMode = !autoMode;
    }
    if(input == IRCODE_B) { 
      MESH_WIREFRAME = !MESH_WIREFRAME;  
    }
    if(input == IRCODE_C) { 
      showClock = !showClock;
    }
    if (input == IRCODE_HOME) {
        isOff = !isOff;

        if (isOff){
            pSmartMatrix->fillScreen(COLOR_BLACK);
            LEDS.show();
        }

        return input;
    }

    return input;
}

void loop() {
  handleInput();
  if(isOff) { 
    delay(100);
    return;
  }
  if(autoMode) {
    if(millis() >= nextMode) { 
      currentMode++;
      initMode = true;
      if(currentMode > maxMode) { currentMode = 0; }
        Serial.print("Current Mode: ");
        Serial.println(currentMode);
      nextMode = millis() + (MODE_TIMEOUT * 1000);
    }
  }
  drawCurrentMode();

}

void drawCurrentMode() { 
  if(millis() >= nextFrame) { 
    switch(currentMode) { 
      case 0:
        drawNoise();
        break;
      case 1:
        drawFunkySpiral();
        break;
      case 2:
        drawIcosahedron();
        break;
      case 3:
        bmpTest();
       
        break;

    }
    if(showClock) { 

      if(currentMode != 3) { 
      drawClockBadge();
      }
    }
    LEDS.show();

  }
}

// Effects
// ========================================
void bmpTest() { 
  //showClock = false;
  if(initMode) { 
    bmp.Draw("/slides/rtype.bmp",0,0);
  }
  nextFrame = millis() + 10;
}

void drawFunkySpiral() { 
  UpdateTimers();
  pSmartMatrix->drawLine(multiTimer[3].count, multiTimer[4].count, multiTimer[0].count, multiTimer[1].count, CRGB(CHSV(multiTimer[2].count,255,255)));
  DimAll(250);
  nextFrame = millis() + 10;
}

void drawClockBadge() { 
  if(clockOption) {
  char str[32];
  sprintf(str,"%02d", hour());
  pSmartMatrix->setFont(gohufontb);
  pSmartMatrix->drawString(3,12,COLOR_WHITE,str);
  sprintf(str,"%02d", minute());
  pSmartMatrix->drawString(18,12,COLOR_WHITE,str);
  if(second()%2) {
    pSmartMatrix->setFont(gohufont);
   pSmartMatrix->drawString(13,13,COLOR_WHITE,":");
  }
} else {
   clock.useWuLine = false;
   clock.hColor = CHSV(0,0,255);
   clock.analogClock(15,15,15);
 }
}

void drawNoise() { 
  if(initMode) { 
    x = random16();
    y = random16();
    z = random16();
    initMode = false;
  }
  static uint8_t ihue=0;
  fillnoise8();
  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      //leds[XY(i,j)] = CHSV(noise[j][i],255,noise[i][j]);
      leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
    }
  }
  ihue++;
  nextFrame = millis() + 10;

}
