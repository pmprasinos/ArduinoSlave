#include <Wire.h>

#include <Adafruit_GFX.h>
#include "Adafruit_SSD1351.h"
#include <SD.h>
#include <SPI.h>
#include <SdFat.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
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

File bmpFile;

int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;


  int btx = 55;
  int bty = 100;



  unsigned long currenttime=millis();
  unsigned long lasttime;
/////////////////////////Controller Name//////////////////////////////////

 

///////////////////////////Serial Declaration//////////////////////////////
#define button 2
#define xAxisPin A3
#define yAxisPin A2




char ScreenState = 0;
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
    delay(500);
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
    delay(100);
    int voltage=getBandgap();
    unsigned long currenttime=millis(); 
   if (currenttime-lasttime >= 300000){
        lasttime=currenttime;    
        Battery();
       }
      
  }

void Icon()
 {  

  char btc = Wire.read();
  

  
  ScreenState = btc;

  int myWidth = tft.width();
  int myHeight = tft.height();

  
 

        switch (ScreenState)   // Actions to be done when message recieved from Beckoff
          {
              case '0':
                
              break;
        
              case '1':
               
                    tft.fillScreen(BLACK);
                    bmpDraw("height.bmp", 33, 25);
                    tft.setCursor(30, 03);
                    tft.setTextColor(BLUE);  
                    tft.setTextSize(1.85);
                    tft.println("Height Mode");
                    
                    //Battery();
                  
                    break;
                
              
 
        
              case '2':
               
                tft.fillScreen(BLACK);
                bmpDraw("home.bmp", 36, 25);
                tft.setCursor(30, 03);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("Homing Mode");
                   
                //Battery();
                break;
               
              
              case '3':
                tft.fillScreen(BLACK);
                bmpDraw("error.bmp", 36, 25);
                tft.setCursor(53, 3);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("Error");
                    
                 //Battery();
              break;
              
              case '4':
                tft.fillScreen(BLACK);
                bmpDraw("path.bmp", 33, 25);
                tft.setCursor(36, 3);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1.85);
                tft.println("Path Mode");
                    
                 //Battery();
              break;
              
              case '5':
                tft.fillScreen(BLACK);
                bmpDraw("reset.bmp", 33, 25);
                tft.setCursor(32, 3);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1.85);
                tft.println("Path Reset");
                   
                //Battery();
              break;
            
              case '6':
                tft.fillScreen(BLACK);
                bmpDraw("shortcut.bmp", 36, 25);
                tft.setCursor(25, 3);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1.85);
                tft.println("Shortcut Mode");
                   
                //Battery();
              break;
            
              case '7':
                tft.fillScreen(BLACK);
                bmpDraw("storage.bmp", 36, 25);
                tft.setCursor(25, 0);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1.85);
                tft.println("Storage Mode");
                   
                //Battery();
              break;
      
      
              case '8':
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
                   
                //Battery();
              break;
              
              case 'A':
             
                tft.fillCircle(33,113, 5,RED);
                tft.fillCircle(120,113,5,RED);
                    
              break;
             
              case 'B':
              
                tft.fillCircle(33,113, 5,GREEN);
                tft.fillCircle(120,113,5,GREEN);
                    
              break;
              
              case 'C':
           
                tft.fillCircle(33,113, 5,GREEN);
                tft.fillCircle(120,113,5,RED);
                    
              break;
             
              case 'D':
           
                tft.fillCircle(33,113, 5,RED);
                tft.fillCircle(120,113,5,GREEN);
                    
              break;
              
              case 'E':      //grey circle
            
                tft.fillCircle(120,9, 5,0x7777);
              break;
              
              case 'F':     //blue circle 
             
                tft.fillCircle(120,9, 5,BLUE);
              break;

              case 'G':     //bluetooth on 
                
                  
              break;

              case 'H':     //bluetooth off 
                
                tft.fillScreen(BLACK);
                bmpDraw("blue.bmp", 36, 25);
                tft.setCursor(29, 90);
                tft.setTextColor(BLUE);  
                tft.setTextSize(1);
                tft.println("No Connection");
                
                  //Battery();
              break;

              
              
             
             
            
             case '9':
             break;
             
            default:
            break;
          
           
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
//#define BUFFPIXEL 30
//
//void bmpDraw(char *filename, uint8_t x, uint8_t y) {
//
//  File     bmpFile;
//  int      bmpWidth, bmpHeight;   // W+H in pixels
//  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
//  uint32_t bmpImageoffset;        // Start of image data in file
//  uint32_t rowSize;               // Not always = bmpWidth; may have padding
//  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
//  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
//  boolean  goodBmp = false;       // Set to true on valid header parse
//  boolean  flip    = true;        // BMP is stored bottom-to-top
//  int      w, h, row, col;
//  uint8_t  r, g, b;
//  uint32_t pos = 0, startTime = millis();
//
//  if((x >= tft.width()) || (y >= tft.height())) return;
//
//  //Serial.println();
//  //Serial.print("Loading image '");
//  //Serial.print(filename);
//  //Serial.println('\'');
//
//  // Open requested file on SD card
//  if ((bmpFile = SD.open(filename)) == NULL) 
//    {
//      Serial.print("File not found");
//      return;
//    }
//
//  // Parse BMP header
//  if(read16(bmpFile) == 0x4D42)             // BMP signature
//  { 
//    Serial.print("File size: ");
//    Serial.println(read32(bmpFile));
//    (void)read32(bmpFile);                  // Read & ignore creator bytes
//    bmpImageoffset = read32(bmpFile);       // Start of image data
//    Serial.print("Image Offset: ");
//    Serial.println(bmpImageoffset, DEC);
//    // Read DIB header
//    Serial.print("Header size: "); 
//    Serial.println(read32(bmpFile));
//    bmpWidth  = read32(bmpFile);
//    bmpHeight = read32(bmpFile);
//    
//    if(read16(bmpFile) == 1)                // # planes -- must be '1'
//      { 
//        bmpDepth = read16(bmpFile);           // bits per pixel
//        Serial.print("Bit Depth: "); 
//        Serial.println(bmpDepth);
//        if((bmpDepth == 24) && (read32(bmpFile) == 0))    // 0 = uncompressed
//          { 
//            goodBmp = true;                      // Supported BMP format -- proceed!
//           
//    
//            // BMP rows are padded (if needed) to 4-byte boundary
//            rowSize = (bmpWidth * 3 + 3) & ~3;
//    
//            // If bmpHeight is negative, image is in top-down order.
//            // This is not canon but has been observed in the wild.
//            if(bmpHeight < 0) 
//              {
//                bmpHeight = -bmpHeight;
//                flip      = false;
//              }
//    
//            // Crop area to be loaded
//            w = bmpWidth;
//            h = bmpHeight;
//            if((x+w-1) >= tft.width())  w = tft.width()  - x;
//            if((y+h-1) >= tft.height()) h = tft.height() - y;
//    
//            for (row=0; row<h; row++)           // For each scanline...
//              { 
//                tft.goTo(x, y+row);  
//      
//                // Seek to start of scan line.  It might seem labor-
//                // intensive to be doing this on every line, but this
//                // method covers a lot of gritty details like cropping
//                // and scanline padding.  Also, the seek only takes
//                // place if the file position actually needs to change
//                // (avoids a lot of cluster math in SD library).
//                if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
//                  pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
//                else     // Bitmap is stored top-to-bottom
//                  pos = bmpImageoffset + row * rowSize;
//                if(bmpFile.position() != pos) 
//                  { // Need seek?
//                    bmpFile.seek(pos);
//                    buffidx = sizeof(sdbuffer); // Force buffer reload
//                  }
//      
//                // optimize by setting pins now
//                for (col=0; col<w; col++)       // For each pixel...
//                  {
//                    // Time to read more pixel data?
//                    if (buffidx >= sizeof(sdbuffer)) 
//                      { // Indeed
//                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
//                        buffidx = 0; // Set index to beginning
//                      }
//        
//                    // Convert pixel from BMP to TFT format, push to display
//                    b = sdbuffer[buffidx++];
//                    g = sdbuffer[buffidx++];
//                    r = sdbuffer[buffidx++];
//        
//                    if(b+g+r !=0) {
//                      tft.drawPixel(x+col, y+row, tft.Color565(r,g,b));
//                    
//                    // optimized!
//                     //tft.pushColor(tft.Color565(r,g,b));
//                     }
//                  } // end pixel
//              } // end scanline
//            Serial.print("Loaded in ");
//            Serial.print(millis() - startTime);
//            Serial.println(" ms");
//          }     // end goodBmp
//        }
//  }
//
//  bmpFile.close();
//  if(!goodBmp) Serial.println("BMP format not recognized.");
//}
//
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
     const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
        // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
       

    delay(50);  // Let mux settle a little to get a more stable A/D conversion
        // Start a conversion  
     ADCSRA |= _BV( ADSC );
        // Wait for it to complete
     while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
        // Scale the value
     int results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L; // calculates for straight line value 
     return results;
}

void Battery(void)
  {
   int voltage=getBandgap();
   int x=0;
   int y=0;
  
   if (voltage >= 310)
     {
       tft.fillRect(x,y,x+10,y+5,BLACK);
      
       tft.drawLine((x+0),(y+4),(x+3),(y+4),BLUE);
     
       tft.drawLine((x+3),(y+4),(x+3),(y+2),BLUE);
    
       tft.drawLine((x+3),(y+2),(x+7),(y+2),BLUE);
    
       tft.drawLine((x+7),(y+2),(x+7),(y+4),BLUE);
     
       tft.drawLine((x+7),(y+4),(x+10),(y+4),BLUE);
    
       tft.drawLine((x+10),(y+4),(x+10),(y+19),BLUE);
    
       tft.drawLine((x+10),(y+19),(x+0),(y+19),BLUE);
   
       tft.drawLine((x+0),(y+19),(x+0),(y+4),BLUE);
       
       tft.fillRect(x+2,y+6,x+6,y+3,GREEN);
       tft.fillRect(x+2,y+10,x+6,y+3,YELLOW);
       tft.fillRect(x+2,y+14,x+6,y+3,RED);
     }

   if(voltage<310 && voltage>302)
     {
      tft.fillRect(x,y,x+10,y+5,BLACK);
      //tft.fillRect(0,0,6,3,GREEN);
      tft.fillRect(x+0,y+4,x+6,y+3,YELLOW);
      tft.fillRect(x+0,y+8,x+6,y+3,RED);
     }

   if(voltage<=302)
     {
      tft.fillRect(x,y,x+10,y+10,BLACK);
    
      tft.fillRect(x+0,y+8,x+6,y+3,RED);
    ;
     
     
   }

   
   
  }
  
#define BUFFPIXEL 30
  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
  uint32_t pos = 0, startTime = millis();
  uint16_t imgArry[16348];            // array that holds all pixel converted into tft format hard coded for 128 x 128 display
  uint32_t pixPos = 0;                // position in the image array 
  if((x >= tft.width()) || (y >= tft.height())) return;
  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }
  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      Serial.println("No. of Planes: 1");
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
    Serial.print("Image size: ");
    Serial.print(bmpWidth);
    Serial.print('x');
    Serial.println(bmpHeight);

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
    if((x+w-1) >= tft.width())  w = tft.width()  - x;
    if((y+h-1) >= tft.height()) h = tft.height() - y;
//        tft.displayImageOnScreen(sdbuffer);
        //========= Read in the BMP image put into imgArry ======== 

    for (row=0; row<h; row++) { // For each scanline...
      // tft.goTo(x, y+row);

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
      if(bmpFile.position() != pos) { // Need seek?
        bmpFile.seek(pos);
        buffidx = sizeof(sdbuffer); // Force buffer reload
      }

      // optimize by setting pins now
      for (col=0; col<w; col++) { // For each pixel...
        // Time to read more pixel data?
        if (buffidx >= sizeof(sdbuffer)) { // Indeed
          bmpFile.read(sdbuffer, sizeof(sdbuffer));
          buffidx = 0; // Set index to beginning
        }

        // Convert pixel from BMP to TFT format, push to display
        b = sdbuffer[buffidx++];
        g = sdbuffer[buffidx++];
        r = sdbuffer[buffidx++];

        imgArry[pixPos] = tft.Color565(r,g,b); 
        pixPos++;
      } // end pixel
    } // end scanline


    Serial.print("Imaged Loaded into array in ");
    Serial.print(millis() - startTime);
    Serial.println(" ms");

    // --- lets write the array out to the OLED screen

    startTime = millis();
    
    tft.displayImageOnScreen(imgArry);

    Serial.print("Imaged Loaded into OLED in ");
    Serial.print(millis() - startTime);
    Serial.println(" ms");
    
  } // end goodBmp
}
  }
  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

uint16_t read16(File f) 
  {
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
  }

uint32_t read32(File f) 
  {
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
  }
