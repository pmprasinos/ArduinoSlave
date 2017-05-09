#include <Servo.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

//#include "Adafruit_SSD1351.h"
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

  unsigned long currenttime=millis();
  unsigned long lasttime;
/////////////////////////Controller Name//////////////////////////////////

 

///////////////////////////Serial Declaration//////////////////////////////
#define button 2
#define xAxisPin A3
#define yAxisPin A2




int ScreenState = 0;
///////////////////Inputs//////////////////////////


///////////////////////////////////////////////////////////////////////////////////////


void setup()
  {

    Serial.begin(9600);
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
          Serial.println("SD Failed");
          return;
      }
    Serial.println("SD OK!");  
    tft.setRotation(2);    // Sets the rotation of the Bitmap images 0,1,2,3,4
     
     tft.fillScreen(BLACK);
                bmpDraw("blue.bmp", 36, 25);
                tft.setCursor(29, 90);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("No Connection");
     
    
     Battery();
  }


void loop()
  {
    
    
     unsigned long currenttime=millis(); 
    int btc = Wire.read();
   ScreenState = btc;
    if (btc > 1 ) Icon(btc);

   if (currenttime-lasttime >= 300000){
        lasttime=currenttime;    
        Battery();
       }
      
  }

void Icon(int btc)
 {  

  

  

   if(btc<=1) 
   {
    return NULL;
   }
  Serial.println(btc);
  ScreenState = btc;

  int myWidth = tft.width();
  int myHeight = tft.height();

  
 

        switch (ScreenState)   // Actions to be done when message recieved from Beckoff
          {
              case 0:
              break;

              case 255:
              break;

              case 1:
              break;

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
               
                tft.fillScreen(BLACK);
                bmpDraw("home.bmp", 36, 25);
                tft.setCursor(30, 03);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("Homing Mode");
                   
                RenderBattery(voltage);
                break;
               
              
              case 51:
                tft.fillScreen(BLACK);
                bmpDraw("error.bmp", 36, 25);
                tft.setCursor(53, 3);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("Error");
                    
                 RenderBattery(voltage);
              break;
              
              case 52:
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
      
      
              case 56:
                tft.fillScreen(BLACK);                
                tft.setCursor(30, 03);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("Homing Mode");
                tft.setCursor(36, 14);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("Tracking");
                bmpDraw("home.bmp", 33, 25);
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
                tft.fillCircle(33,113, 5,RED);
                tft.fillCircle(120,113,5,RED);
                   
                RenderBattery(voltage);
              break;
              
              case 65:
             
                tft.fillCircle(33,113, 5,RED);
                tft.fillCircle(120,113,5,RED);
                    
              break;
             
              case 66:
              
                tft.fillCircle(33,113, 5,GREEN);
                tft.fillCircle(120,113,5,GREEN);
                    
              break;
              
              case 67:
           
                tft.fillCircle(33,113, 5,GREEN);
                tft.fillCircle(120,113,5,RED);
                    
              break;
             
              case 68:
           
                tft.fillCircle(33,113, 5,RED);
                tft.fillCircle(120,113,5,GREEN);
                    
              break;
              
              case 69:      //grey circle
            
                tft.fillCircle(120,9, 5,0x7777);
              break;
              
              case 70:     //blue circle 
             
                tft.fillCircle(120,9, 5,BLUE);
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
              break;

            
             
            default:
            

            break;
           Serial.println(ScreenState);
           ScreenState = 0;
    }
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
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;
  uint32_t startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;
  startTime = millis();
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) 
    {
      Serial.print("File not found");
      return;
    }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42)             // BMP signature
  { 
    Serial.print("File size: ");
    Serial.println(read32(bmpFile));
    (void)read32(bmpFile);                  // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    Serial.print("Image Offset: ");
    Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); 
    Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    
    if(read16(bmpFile) == 1)                // # planes -- must be '1'
      { 
        bmpDepth = read16(bmpFile);           // bits per pixel
        Serial.print("Bit Depth: "); 
        Serial.println(bmpDepth);
        if((bmpDepth == 24) && (read32(bmpFile) == 0))    // 0 = uncompressed
          { 
            goodBmp = true;                      // Supported BMP format -- proceed!
           
    
            // BMP rows are padded (if needed) to 4-byte boundary
            rowSize = (bmpWidth * 3 + 3) & ~3;
    
            // If bmpHeight is negative, image is in top-down order.
            // This is not canon but has been observed in the wild.
            if(bmpHeight < 0) 
              {
                bmpHeight = -bmpHeight;
                flip      = false;
              }
    
            // Crop area to be loaded
            w = bmpWidth;
            h = bmpHeight;
            if((x+w-1) >= tft.width())  w = tft.width()  - x;
            if((y+h-1) >= tft.height()) h = tft.height() - y;
    
            for (row=0; row<h; row++)           // For each scanline...
              { 
                tft.goTo(x, y+row);  
      
                // Seek to start of scan line.  It might seem labor-
                // intensive to be doing this on every line, but this
                // method covers a lot of gritty details like cropping
                // and scanline padding.  Also, the seek only takes
                // place if the file position actually needs to change
                // (avoids a lot of cluster math in SD library).
                if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                  pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                else     // Bitmap is stored top-to-bottom
                  pos = bmpImageoffset + row * rowSize;
                if(bmpFile.position() != pos) 
                  { // Need seek?
                    bmpFile.seek(pos);
                    buffidx = sizeof(sdbuffer); // Force buffer reload
                  }
      
                // optimize by setting pins now
                for (col=0; col<w; col++)       // For each pixel...
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
        
                    if(b+g+r > 100) {
                      tft.drawPixel(x+col, y+row, tft.Color565(r,g,b));
                    
                    // optimized!
                     //tft.pushColor(tft.Color565(r,g,b));
                     }
                  } // end pixel
              } // end scanline
            Serial.print("Loaded in ");
            Serial.print(millis() - startTime);
            Serial.println(" ms");
          }     // end goodBmp
        }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
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
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result;
}

void Battery(void)
  {
  voltage=10000;
   //RenderBattery(voltage);
   //Serial.print("VOLTAGE: ");
   //Serial.println(voltage);
  }

void RenderBattery(int voltage)
{
    int x=0;
   int y=0;
   
        tft.fillRect(14    ,0      , 20 , 5  , BLACK);
        tft.fillRect(x    ,y      , 10 , 19  , BLACK);
  
        tft.fillRect(x+2  ,y+15   , 6  , 4  , RED);
        tft.fillRect(x+2  ,y+10   , 6  , 4  , YELLOW);
        tft.fillRect(x+2  ,y+5    , 6  , 4  , GREEN);
       
       
       if(voltage < 385)
       {
          tft.fillRect(x+2, y+4, 6,  (( 3120 - voltage)/12), BLACK);
       }
       
      
       tft.drawLine((x+0),(y+4),(x+3),(y+4),BLUE);
     
       tft.drawLine((x+3),(y+4),(x+3),(y+2),BLUE);
    
       tft.drawLine((x+3),(y+2),(x+7),(y+2),BLUE);
    
       tft.drawLine((x+7),(y+2),(x+7),(y+4),BLUE);
     
       tft.drawLine((x+7),(y+4),(x+10),(y+4),BLUE);
    
       tft.drawLine((x+10),(y+4),(x+10),(y+20),BLUE);
    
       tft.drawLine((x+10),(y+20),(x+0),(y+20),BLUE);
   
       tft.drawLine((x+0),(y+20),(x+0),(y+4),BLUE);


      
        tft.setCursor(14, 0);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println(voltage);
      
    ;
     
     

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


