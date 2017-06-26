#include <Servo.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "SdFat.h"
#include <SPI.h>
//#include <Fonts/FreeMonoBoldOblique12pt7b.h>
///////////////////OLED Things////////////////////////

#define sclk 13
#define mosi 11
#define dc   A1
#define cs   9
#define rst  A0
#define miso   12
#define SD_CS 10

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

//File bmpFile;
SdFat SD;

int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;


int btx = 55;
int bty = 100;
//SdFat sd;
int voltage;  //moved voltage to global so that battery can be checked and rendered seporately

bool targetMode = false;
unsigned long currenttime = millis();
unsigned long lasttime;
/////////////////////////Controller Name//////////////////////////////////

///////////////////////////Serial Declaration//////////////////////////////
#define button 2
#define xAxisPin A3
#define yAxisPin A2


int btcg;
int ShapeStateDeadMan = 0;  //for smaller objects like Estop, and interlocks
int ShapeStateInterLock = 0;
int ShapeStateCarrier = 0;
int btc = 0;
int ScreenState = 0;

bool runthistime = false;

int loopcount;
bool debug = true;

void setup()
{

  if (debug) Serial.begin(9600);
  Wire.begin(1);
  Wire.onReceive(Icon);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);

  tft.begin();

  tft.fillScreen(BLACK);

  if (!SD.begin(SD_CS))   // Check SD Card
  {
    tft.setCursor(30, 60);
    tft.setTextColor(RED);
    tft.setTextSize(1.85);
    if (debug) Serial.println("SD Failed");
    return;
  }
  if (debug) Serial.println("SD OK!");
  tft.setRotation(2);    // Sets the rotation of the Bitmap images 0,1,2,3,4

  
    
  bmpDraw("blue.bmp", 36, 25);
 tft.setCursor(29, 90);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.println("No Connection");

  ScreenState = 72;
  Battery();
}


void loop()
{

  //Serial.print(loopcount);
  loopcount++;
  unsigned long currenttime = millis();
  delay(1);

  if (currenttime - lasttime >= 300000)
  {
    lasttime = currenttime;
    Battery();
  }
  if (btc != -1) Icon(0);
}

void Icon(int numBytes)
{
  delay(1);
  int btcg = -1;
  if (numBytes > 0) btcg = Wire.read();

  if (btcg != -1) {
    runthistime = true;
    btc = btcg;
  }


  if (runthistime && numBytes == 0)
  {
    runthistime = false;

    if (debug) Serial.println(btc);
    if(ScreenState == btc) return;
    ScreenState = btc;

    int myWidth = tft.width();
    int myHeight = tft.height();

     int xOffset = 0;
    int yOffset = -10;
    
    switch (ScreenState)   // Actions to be done when message recieved from Beckoff
    {

      case 39:
        break;

      case 49:

        tft.fillScreen(BLACK);
        bmpDraw("height.bmp", 33, 25);
        tft.setCursor(30, 03);
        tft.setTextColor(BLUE);
        tft.setTextSize(1.85);
        tft.println("Height Mode");

        RenderBattery(voltage);

        break;

      case 50:
         targetMode = false;
        tft.fillScreen(BLACK);
        bmpDraw("home.bmp", 36, 25);
        tft.setCursor(30, 03);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("Homing Mode");

        RenderBattery(voltage);
        break;


      case 51:
         targetMode = false;
        tft.fillScreen(BLACK);
        bmpDraw("error.bmp", 36, 25);
        tft.setCursor(53, 3);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("Error");

        RenderBattery(voltage);
        break;

      case 52:
         targetMode = false;
        tft.fillScreen(BLACK);
        bmpDraw("path.bmp", 33, 25);
        tft.setCursor(36, 3);
        tft.setTextColor(BLUE);
        tft.setTextSize(1.85);
        tft.println("Path Mode");
        RenderBattery(voltage);
        break;

      case 53:
        tft.fillScreen(BLACK);
        bmpDraw("reset.bmp", 33, 25);
        tft.setCursor(32, 3);
        tft.setTextColor(BLUE);
        tft.setTextSize(1.85);
        tft.println("Path Reset");

        RenderBattery(voltage);
        break;

      case 54:
        tft.fillScreen(BLACK);
        bmpDraw("shortcut.bmp", 36, 25);
        tft.setCursor(25, 3);
        tft.setTextColor(BLUE);
        tft.setTextSize(1.85);
        tft.println("Shortcut Mode");

        RenderBattery(voltage);
        break;

      case 55:
        tft.fillScreen(BLACK);
        bmpDraw("storage.bmp", 36, 25);
        tft.setCursor(25, 0);
        tft.setTextColor(BLUE);
        tft.setTextSize(1.85);
        tft.println("Storage Mode");

        RenderBattery(voltage);
        break;


      case 56:                 //ASCII 8
        tft.fillScreen(BLACK);
        runthistime = true;
        bmpDraw("home.bmp", 33, 25);
        btc = 57;
        break;

      case 57:
        targetMode = false;
        tft.setCursor(30, 03);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("Homing Mode");
        tft.setCursor(36, 14);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("Tracking");
        tft.setCursor(0, 87);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("Carrier");
        tft.setCursor(98, 87);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("Lock");
        bmpDraw("carrier.bmp", 0, 100);
        bmpDraw("lock.bmp", 93, 100);

        tft.fillCircle(33, 113, 5, RED);
        tft.fillCircle(120, 113, 5, RED);
        RenderBattery(voltage);
        break;

      case 65:
        tft.fillCircle(33, 113, 5, RED);
        tft.fillCircle(120, 113, 5, RED);
        break;

      case 66:
        tft.fillCircle(33, 113, 5, GREEN);
        tft.fillCircle(120, 113, 5, GREEN);
        break;

      case 67:
        tft.fillCircle(33, 113, 5, GREEN);
        tft.fillCircle(120, 113, 5, RED);
        break;

      case 68:
        tft.fillCircle(33, 113, 5, RED);
        tft.fillCircle(120, 113, 5, GREEN);
        break;

      case 69:      //grey circle for deadman
        tft.fillCircle(120, 9, 5, 0x7777);
        ShapeStateDeadMan = 0;
        break;

      case 70:     //blue circle for deadman    
        tft.fillCircle(120, 9, 5, BLUE);
        ShapeStateDeadMan = 1;

        break;

      case 71:     //bluetooth on

        break;

      case 72:     //bluetooth off
      tft.fillScreen(BLACK);
        bmpDraw("blue.bmp", 36, 25);
        tft.setCursor(29, 90);
        tft.setTextColor(BLUE);
        tft.setTextSize(1);
        tft.println("No Connection");
        RenderBattery(voltage);
        targetMode = false;
        break;

      case 73: //targeting Mode
         if(targetMode == false)  
         {
          tft.fillRect(10, 10, 105, 105, BLACK);
          tft.drawCircle(64 + xOffset, 74 + yOffset , 32, BLUE);
          tft.drawCircle(64 + xOffset, 74 + yOffset , 33, BLUE);
           tft.setCursor(32 , 3  );
           tft.setTextColor(BLUE);
          tft.setTextSize(1);
          tft.println("Target Mode");
          RenderBattery(1153);
         }
          tft.fillRect(64 + xOffset, 89 + yOffset , 2, 18, BLACK); //horizontal
         tft.fillRect(64 + xOffset, 42 + yOffset , 2, 18, BLACK);
         tft.fillRect(79 + xOffset, 74 + yOffset , 18, 2, BLACK); //vertical
         tft.fillRect(32 + xOffset, 74 + yOffset , 18, 2, BLACK);
         
         
         tft.fillRect(64 + xOffset, 107 + yOffset , 2, 15, BLUE);
         tft.fillRect(64 + xOffset, 26 + yOffset , 2, 15, BLUE);
         tft.fillRect(97 + xOffset, 74 + yOffset , 15, 2, BLUE);
         tft.fillRect(16 + xOffset, 74 + yOffset , 15, 2, BLUE);
         tft.fillCircle(64 + xOffset, 74 + yOffset , 7, BLACK);
                   tft.setTextColor(BLACK);
          tft.setTextSize(1);
           tft.setCursor(8, 118 );
           tft.print("Hold MD for Shadow" );
         targetMode = true;
         break;

      case 74:
       if(targetMode == false)  
         {
          tft.fillScreen(BLACK);
          tft.drawCircle(64 + xOffset, 74 + yOffset , 32, BLUE);
          tft.drawCircle(64 + xOffset, 74 + yOffset , 33, BLUE);
           tft.setCursor(32 , 3  );
           tft.setTextColor(BLUE);
          tft.setTextSize(1);
          tft.println("Target Mode");
          targetMode = true;
          RenderBattery(1153);
         }
          tft.fillRect(64 + xOffset, 107 + yOffset , 2, 15, BLACK);
         tft.fillRect(64 + xOffset, 26 + yOffset , 2, 15, BLACK);
         tft.fillRect(98 + xOffset, 74 + yOffset , 14, 2, BLACK);
         tft.fillRect(16 + xOffset, 74 + yOffset , 15, 2, BLACK);
         
         tft.fillRect(64 + xOffset, 89 + yOffset , 2, 18, BLUE); //horizontal
         tft.fillRect(64 + xOffset, 42 + yOffset , 2, 18, BLUE);
         tft.fillRect(79 + xOffset, 74 + yOffset , 18, 2, BLUE); //vertical
         tft.fillRect(32 + xOffset, 74 + yOffset , 18, 2, BLUE);
         tft.fillCircle(64 + xOffset, 74 + yOffset , 7, GREEN);
         
          tft.setTextColor(BLUE);
          tft.setTextSize(1);
           tft.setCursor(8, 118 );
           tft.print("Hold MD for Shadow" );
         break;

      case 75:
         removeTarget('1');

         break;

         case 76:

        removeTarget('2');

         break;

         case 77:
       
        removeTarget('3');

         break;

         case 78:

        removeTarget('4');

         break;
         
      default:

        break;
        if (debug) Serial.println(ScreenState);

    }

  }
  
}

void removeTarget(char num)
{
  targetMode = false;
      tft.fillCircle(64, 74-10, 7, BLACK);
       tft.drawCircle(64, 74-10, 32, BLACK);
          tft.drawCircle(64, 74-10, 33, BLACK);
           tft.setCursor(32, 3);
           tft.setTextColor(BLACK);
          tft.setTextSize(1);
          tft.print("Target Mode");
           tft.setCursor(8, 118 );
           tft.print("Hold MD for Shadow" );
             tft.setTextColor(BLUE);
           tft.setCursor(27, 03);
           tft.println("Shadow Active");
           tft.setCursor(8, 120);
           tft.print("Hold TR+MD to exit" );
           
               tft.fillRect(64, 89-10, 2, 18, BLACK); //horizontal
         tft.fillRect(64, 42-10, 2, 18, BLACK);
         tft.fillRect(79, 74-10, 18, 2, BLACK); //vertical
         tft.fillRect(32, 74-10, 18, 2, BLACK);
          tft.setCursor(59, 34);
          tft.setTextColor(GREEN);
          tft.setTextSize(2);
          tft.print(num);
           bmpDraw("shadow.bmp", 28, 18);
           RenderBattery(1153);
}

/////////////////////////////BitMap Drawing Fucntion///////////////////////
//// This function opens a Windows Bitmap (BMP) file and
//// displays it at the given coordinates.  It's sped up
//// by reading many pixels worth of data at a time
//// (rather than pixel by pixel).  Increasing the buffer
//// size takes more of the Arduino's precious RAM but
//// makes loading a little faster.  20 pixels seems a
//// good balance.
//
#define BUFFPIXEL 25

void bmpDraw(char *filename, uint8_t x, uint8_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;
  uint32_t startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;
  startTime = millis();
  if (debug) Serial.println();
  if (debug) Serial.print("Loading image '");
  if (debug) Serial.print(filename);
  if (debug) Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL)
  {
    if (debug) Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42)            // BMP signature
  {
    if (debug) Serial.print("File size: ");
    if (debug) Serial.println(read32(bmpFile));
    (void)read32(bmpFile);                  // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    if (debug) Serial.print("Image Offset: ");
    if (debug) Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    if (debug) Serial.print("Header size: ");
    if (debug) Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    if (read16(bmpFile) == 1)               // # planes -- must be '1'
    {
      bmpDepth = read16(bmpFile);           // bits per pixel
      if (debug) Serial.print("Bit Depth: ");
      if (debug) Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0))   // 0 = uncompressed
      {
        goodBmp = true;                      // Supported BMP format -- proceed!


        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0)
        {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        for (row = 0; row < h; row++)       // For each scanline...
        {
          tft.goTo(x, y + row);
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos)
          { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          // optimize by setting pins now
          for (col = 0; col < w; col++)   // For each pixel...
          {
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer))
            { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];

            if (b + g + r > 100) {
              tft.drawPixel(x + col, y + row, tft.Color565(r, g, b));

              // optimized!
              //tft.pushColor(tft.Color565(r,g,b));
            }
          } // end pixel
        } // end scanline
        if (debug) Serial.print("Loaded in ");
        if (debug) Serial.print(millis() - startTime);
        if (debug) Serial.println(" ms");
      }     // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp)  Serial.println("BMP format not recognized.");



}

//// These read 16- and 32-bit types from the SD card file.
//// BMP data is stored little-endian, Arduino is little-endian too.
//// May need to reverse subscript order if porting elsewhere.
//
//uint16_t read16(File f)
//  {
//    uint16_t result;
//    ((uint8_t *)&result)[0] = f.read(); // LSB
//    ((uint8_t *)&result)[1] = f.read(); // MSB
//    return result;
//  }
//
//uint32_t read32(File f)
//  {
//    uint32_t result;
//    ((uint8_t *)&result)[0] = f.read(); // LSB
//    ((uint8_t *)&result)[1] = f.read();
//    ((uint8_t *)&result)[2] = f.read();
//    ((uint8_t *)&result)[3] = f.read(); // MSB
//    return result;
//  }
//////////////////////////////////
//






/////////////////////Battery Checking Function//////////////////////////////

int getBandgap(void) // Returns actual value of Vcc (x 100)
{
  // For 168/328 boards
  const long InternalReferenceVoltage = 1306L;  // Adjust this value to your boards specific internal BG voltage x1000 //updated 5/9/2017 to reflect current board
  // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
  //MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  //  #endif

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high << 8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result;
}

void Battery(void)
{
  voltage = 10000;
  //RenderBattery(voltage);
  if (debug) Serial.print("VOLTAGE: ");
  if (debug) Serial.println(voltage);
}

void RenderBattery(int voltage)
{
  int x = 0;
  int y = 0;

 // tft.fillRect(14    , 0      , 20 , 5  , BLACK);
  tft.fillRect(x    , y      , 10 , 19  , BLACK);

  tft.fillRect(x + 2  , y + 15   , 6  , 4  , RED);
  tft.fillRect(x + 2  , y + 10   , 6  , 4  , YELLOW);
  tft.fillRect(x + 2  , y + 5    , 6  , 4  , GREEN);


  if (voltage < 385)
  {
    tft.fillRect(x + 2, y + 4, 6,  (( 3120 - voltage) / 12), BLACK);
  }


  tft.drawLine((x + 0), (y + 4), (x + 3), (y + 4), BLUE);
  tft.drawLine((x + 3), (y + 4), (x + 3), (y + 2), BLUE);
  tft.drawLine((x + 3), (y + 2), (x + 7), (y + 2), BLUE);
  tft.drawLine((x + 7), (y + 2), (x + 7), (y + 4), BLUE);
  tft.drawLine((x + 7), (y + 4), (x + 10), (y + 4), BLUE);
  tft.drawLine((x + 10), (y + 4), (x + 10), (y + 20), BLUE);
  tft.drawLine((x + 10), (y + 20), (x + 0), (y + 20), BLUE);
  tft.drawLine((x + 0), (y + 20), (x + 0), (y + 4), BLUE);

  // tft.setCursor(14, 0);
  //         tft.setTextColor(BLUE);
  //         tft.setTextSize(1);
  //         tft.println(voltage);
}


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}



