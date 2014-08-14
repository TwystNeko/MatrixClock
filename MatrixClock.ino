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


SdFat sd;
SdFile file;

const byte numParticles = 30;



#define kMatrixWidth  32
#define kMatrixHeight 32

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

// -----------------------------------------------------------
// Spinning Cube from Peggy2 Library
// -----------------------------------------------------------

float focal = 30; // Focal of the camera
int cubeWidth = 28; // Cube size
float Angx = 0, AngxSpeed = 0.017; // rotation (angle+speed) around X-axis
float Angy = 0, AngySpeed = 0.022; // rotation (angle+speed) around Y-axis
float Ox=15.5,Oy=15.5; // position (x,y) of the frame center
int zCamera = 110; // distance from cube to the eye of the camera

unsigned short reps = 0; // used for grey scaling
unsigned short repNumber = 1;  //Change scrolling rate-- number of reps at each position.

// ------------------------------------------------------------
// struct Vertex
// ------------------------------------------------------------
struct Vertex
{
  float x,y,z;  
  Vertex()
  {
    this->set(0,0,0);
  }

  Vertex(float x,float y, float z)
  {
    this->set(x,y,z);
  }

  void set(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

// ------------------------------------------------------------
// struct EdgePoint
// ------------------------------------------------------------
struct EdgePoint
{
  int x,y;
  boolean visible;

  EdgePoint()
  {
    this->set(0,0);
    this->visible = false;
  }

  void set(int a,int b)
  {
    this->x = a;
    this->y = b;
  }
};

// ------------------------------------------------------------
// struct Point
// ------------------------------------------------------------
struct Point
{      
  float x,y;

  Point()
  {
    set(0,0);
  }

  Point(float x,float y)
  {
    set(x,y);
  }

  void set(float x, float y)
  {
    this->x = x;
    this->y = y;    
  }

};      

// ------------------------------------------------------------
// struct squareFace
// ------------------------------------------------------------
struct squareFace
{      
  int length;
  int sommets[4];
  int ed[4];

  squareFace()
  {
    set(-1,-1,-1,-1);
  }

  squareFace(int a,int b,int c,int d)
  {
    this->length = 4;
    this->sommets[0]=a;
    this->sommets[1]=b;
    this->sommets[2]=c;
    this->sommets[3]=d;
  }

  void set(int a,int b,int c,int d)
  {
    this->length = 4;
    this->sommets[0]=a;
    this->sommets[1]=b;
    this->sommets[2]=c;
    this->sommets[3]=d;
  }

};

// ------------------------------------------------------------
// struct Cube
// ------------------------------------------------------------
struct Cube
{
  // Local vertices
  Vertex  local[8];
  // Camera aligned vertices
  Vertex  aligned[8];
  // On-screen projected vertices
  Point   screen[8];
  // Faces
  squareFace face[6];
  // Edges
  EdgePoint edge[12];
  int nbEdges;
  // ModelView matrix
  float m00,m01,m02,m10,m11,m12,m20,m21,m22;  

  // constructor
  Cube(){}

  // constructs the cube
  void make(int w)
  {
    nbEdges = 0;

    local[0].set(-w,w,w);
    local[1].set(w,w,w);
    local[2].set(w,-w,w);
    local[3].set(-w,-w,w);
    local[4].set(-w,w,-w);
    local[5].set(w,w,-w);
    local[6].set(w,-w,-w);
    local[7].set(-w,-w,-w);  

    face[0].set(1,0,3,2);
    face[1].set(0,4,7,3);
    face[2].set(4,0,1,5);
    face[3].set(4,5,6,7);
    face[4].set(1,2,6,5);
    face[5].set(2,3,7,6);

    int f,i;
    for (f=0;f<6;f++)
    {  
      for (i=0;i<face[f].length;i++)
      {
        face[f].ed[i]= this->findEdge(face[f].sommets[i],face[f].sommets[i?i-1:face[f].length-1]);
      }
    }

  }

  // finds edges from faces
  int findEdge(int a,int b)
  {    
    int i;
    for (i=0;i<nbEdges;i++)
      if ( (edge[i].x==a && edge[i].y==b) || (edge[i].x==b && edge[i].y==a))
        return i;
    edge[nbEdges++].set(a,b);
    return i;
  }

  // rotates according to angle x&y
  void rotate(float angx, float angy)
  {
    int i,j;
    int a,b,c;
    float cx=cos(angx);
    float sx=sin(angx);
    float cy=cos(angy);
    float sy=sin(angy);

    m00=cy;  
    m01=0;  
    m02=-sy;
    m10=sx*sy;
    m11=cx;
    m12=sx*cy;
    m20=cx*sy;
    m21=-sx;
    m22=cx*cy;

    for (i=0;i<8;i++)
    {  
      aligned[i].x=m00*local[i].x+m01*local[i].y+m02*local[i].z;
      aligned[i].y=m10*local[i].x+m11*local[i].y+m12*local[i].z;
      aligned[i].z=m20*local[i].x+m21*local[i].y+m22*local[i].z+zCamera;

      screen[i].x = floor((Ox+focal*aligned[i].x/aligned[i].z));
      screen[i].y = floor((Oy-focal*aligned[i].y/aligned[i].z));        
    }          

    for (i=0;i<12;i++)
      edge[i].visible=false;

    Point *pa,*pb,*pc;
    for (i=0;i<6;i++)
    {  
      pa=screen + face[i].sommets[0];  
      pb=screen + face[i].sommets[1];  
      pc=screen + face[i].sommets[2];  

      boolean back=((pb->x-pa->x)*(pc->y-pa->y)-(pb->y-pa->y)*(pc->x-pa->x))<0;
      if (!back)
      {
        int j;
        for (j=0;j<4;j++)
        {      
          edge[face[i].ed[j]].visible=true;
        }
      }      
    }
  }

  // Draw the cube using the line method !
  void draw(int hue)
  {
     int i;

    // Backface
    EdgePoint *e;
    for (i=0;i<12;i++)
    {  
      e = edge+i;
      if (!e->visible)
        pSmartMatrix->drawLine(screen[e->x].x,screen[e->x].y,screen[e->y].x,screen[e->y].y,CRGB(CHSV(hue,255,128)));  
    }
for (i=0;i<12;i++)
    {  
      e = edge+i;
      if (e->visible)
      {
        pSmartMatrix->drawLine(screen[e->x].x,screen[e->x].y,screen[e->y].x,screen[e->y].y,CRGB(CHSV(hue,255,255)));
      }
    }

  }

};

Cube cube;

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

  Serial.print("Initializing SD card...");
  if (!sd.begin(SD_CS, SPI_FULL_SPEED)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("SD OK!");


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
        drawCube();
        break;
      case 3:
        bmpTest();
       
        break;
    }
    if(showClock) { 
    //  if(currentMode != 3) { 
      drawClockBadge();
  //    }
    }
    LEDS.show();
  }
}

// Effects
// ========================================
void bmpTest() { 
  //showClock = false;
  if(initMode) { 
    bmp.Draw("/slides/entslogo.bmp",0,0);
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


void drawCube() { 

  if(initMode) {
    cube.make(cubeWidth);
    initMode = false;
  }
  pSmartMatrix->fillScreen(COLOR_BLACK);
  Angx+= AngxSpeed;
  Angy+= AngySpeed;
  if(Angx>=TWO_PI) Angx-=TWO_PI;
  if(Angy>=TWO_PI) Angy-=TWO_PI;
  cube.rotate(Angx,Angy);
  cube.draw(cubeHue);
  cubeHue++;

  nextFrame = millis() + 10;
}
