#include "font.h"
#include "touchlcd.h"
#include "Arduino.h"
// SPI setup that should probably move
#include <SPI.h>

#ifdef ALTERNATE_PINS
  #define HSPI_MISO   26
  #define HSPI_MOSI   27
  #define HSPI_SCLK   25
  #define HSPI_SS     32
#else
  #define HSPI_MISO   12
  #define HSPI_MOSI   13
  #define HSPI_SCLK   14
  #define HSPI_SS     15
#endif

#define LCD_LATCH 27

///static const int spiClk = 20000000; // 20 MHz
static const int spiClk = 20 * 1000000;

SPIClass * hspi = NULL;

// End SPI setup

/*
 * ███████ ███████ ████████ ██    ██ ██████  
 * ██      ██         ██    ██    ██ ██   ██ 
 * ███████ █████      ██    ██    ██ ██████  
 *      ██ ██         ██    ██    ██ ██      
 * ███████ ███████    ██     ██████  ██      
 */
TouchLCD::TouchLCD(void) {

  // SPI init
  hspi = new SPIClass(HSPI); 
//#ifndef ALTERNATE_PINS
  hspi->begin();
//#else
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS
//#endif

  pinMode(LCD_LATCH, OUTPUT); //HSPI SS
    digitalWrite(LCD_LATCH, LOW);
  // end SPI init
  
//  pinMode(LCD_RD, OUTPUT);/
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
//  pinMode(LCD_CS, OUTPUT);/
  pinMode(LCD_REST, OUTPUT);
//  digitalWrite(LCD_RD, HIGH);/
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RS, HIGH);
//  digitalWrite(LCD_CS, HIGH);/
  digitalWrite(LCD_REST, HIGH);


//  for(int i = 0; i < BIT_COUNT; i++) {
//    pinMode(bits[i], OUTPUT);
//  }
  
  digitalWrite(LCD_REST,HIGH);
  delay(5); 
  digitalWrite(LCD_REST,LOW);
  delay(15);
  digitalWrite(LCD_REST,HIGH);
  delay(15);

//  digitalWrite(LCD_CS,HIGH);/
//  digitalWrite(LCD_WR,HIGH);/
//  digitalWrite(LCD_CS,LOW);  ///CS

  Lcd_Write_Com(0xF9);  
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x08);
  Lcd_Write_Com(0xC0);  
  Lcd_Write_Data(0x19); 
  Lcd_Write_Data(0x1A);  
  Lcd_Write_Com(0xC1);  
  Lcd_Write_Data(0x45); 
  Lcd_Write_Data(0X00); 
  Lcd_Write_Com(0xC2);  
  Lcd_Write_Data(0x33); 
  Lcd_Write_Com(0xC5);  
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x28); 
  Lcd_Write_Com(0xB1);  
  Lcd_Write_Data(0x90); 
  Lcd_Write_Data(0x11); 
  Lcd_Write_Com(0xB4);  
  Lcd_Write_Data(0x02); 
  Lcd_Write_Com(0xB6);  
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x42);
  Lcd_Write_Data(0x3B);
  Lcd_Write_Com(0xB7);  
  Lcd_Write_Data(0x07); 
  Lcd_Write_Com(0xE0); 
  Lcd_Write_Data(0x1F); 
  Lcd_Write_Data(0x25); 
  Lcd_Write_Data(0x22); 
  Lcd_Write_Data(0x0B); 
  Lcd_Write_Data(0x06); 
  Lcd_Write_Data(0x0A); 
  Lcd_Write_Data(0x4E); 
  Lcd_Write_Data(0xC6); 
  Lcd_Write_Data(0x39); 
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x00); 
  Lcd_Write_Data(0x00); 
  Lcd_Write_Com(0xE1); 
  Lcd_Write_Data(0x1F); 
  Lcd_Write_Data(0x3F); 
  Lcd_Write_Data(0x3F); 
  Lcd_Write_Data(0x0F); 
  Lcd_Write_Data(0x1F); 
  Lcd_Write_Data(0x0F); 
  Lcd_Write_Data(0x46); 
  Lcd_Write_Data(0x49); 
  Lcd_Write_Data(0x31); 
  Lcd_Write_Data(0x05); 
  Lcd_Write_Data(0x09); 
  Lcd_Write_Data(0x03); 
  Lcd_Write_Data(0x1C); 
  Lcd_Write_Data(0x1A); 
  Lcd_Write_Data(0x00);  
  Lcd_Write_Com(0xF1);   
  Lcd_Write_Data(0x36);   
  Lcd_Write_Data(0x04);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x3C);
  Lcd_Write_Data(0x0F);
  Lcd_Write_Data(0x0F);
  Lcd_Write_Data(0xA4);
  Lcd_Write_Data(0x02);
  Lcd_Write_Com(0xF2);   
  Lcd_Write_Data(0x18);   
  Lcd_Write_Data(0xA3);
  Lcd_Write_Data(0x12);
  Lcd_Write_Data(0x02);
  Lcd_Write_Data(0x32);
  Lcd_Write_Data(0x12);
  Lcd_Write_Data(0xFF);
  Lcd_Write_Data(0x32);
  Lcd_Write_Data(0x00);
  Lcd_Write_Com(0xF4);   
  Lcd_Write_Data(0x40);   
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x91);
  Lcd_Write_Data(0x04);     
  Lcd_Write_Com(0xF8);   
  Lcd_Write_Data(0x21);   
  Lcd_Write_Data(0x04);
  Lcd_Write_Com(0x36);   
  Lcd_Write_Data(0b01101000); // default 0x48   
  Lcd_Write_Com(0x3A);   
  Lcd_Write_Data(0x55);        
 
  Lcd_Write_Com(0x11);    //Exit Sleep 
  delay(120);         
  Lcd_Write_Com(0x29);    //Display on 
}

/*
 *  ██████  ██████  ███    ███ ███    ███ ███████ 
 * ██      ██    ██ ████  ████ ████  ████ ██      
 * ██      ██    ██ ██ ████ ██ ██ ████ ██ ███████ 
 * ██      ██    ██ ██  ██  ██ ██  ██  ██      ██ 
 *  ██████  ██████  ██      ██ ██      ██ ███████ 
 */

void TouchLCD::Lcd_Writ_Bus(unsigned char d) {
  digitalWrite(LCD_LATCH, LOW);
  digitalWrite(LCD_WR, LOW);
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  hspi->transfer(d);
  hspi->endTransaction();
//  for(int i = 0; i < BIT_COUNT; i++) {
//    digitalWriteFast(bits[i], d & 1);
//    d >>= 1;
//  }
  digitalWrite(LCD_WR, LOW);
  // May need some delay?
  digitalWrite(LCD_LATCH, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_LATCH, LOW);
}

void TouchLCD::Lcd_Write_Com(unsigned char VH) {
  digitalWrite(LCD_RS, LOW);
  Lcd_Writ_Bus(VH);
}

void TouchLCD::Lcd_Write_Data(unsigned char VH) {
  digitalWrite(LCD_RS, HIGH);
  Lcd_Writ_Bus(VH);
}

void TouchLCD::Lcd_Write_Com_Data(unsigned char com,unsigned char dat) {
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void TouchLCD::Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) {
//  Serial.printf("%d,%d to %d,%d\n", x1, y1, x2, y2);/
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(x1>>8);
  Lcd_Write_Data(x1);
  Lcd_Write_Data(x2>>8);
  Lcd_Write_Data(x2);
  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(y1>>8);
  Lcd_Write_Data(y1);
  Lcd_Write_Data(y2>>8);
  Lcd_Write_Data(y2);
  Lcd_Write_Com(0x2c);
}

void TouchLCD::sleep() {
  digitalWrite(LCD_LATCH, LOW);
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  hspi->transfer(0);
  hspi->endTransaction();
  digitalWrite(LCD_LATCH, HIGH);
  digitalWrite(LCD_LATCH, LOW);
}

// something new

/*
 * ██████  ██████   █████  ██     ██ 
 * ██   ██ ██   ██ ██   ██ ██     ██ 
 * ██   ██ ██████  ███████ ██  █  ██ 
 * ██   ██ ██   ██ ██   ██ ██ ███ ██ 
 * ██████  ██   ██ ██   ██  ███ ███  
 */

int TouchLCD::RGB(uint8_t r, uint8_t g, uint8_t b) {
  return (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
}

char TouchLCD::hex(int i) {
  i = i % 16;
  return (char) ((i < 10) ? (i+'0') : (i-10+'A'));
}

// Call this first, then call writePixel for each pixel
void TouchLCD::setupPixelWrite(int x, int y, int w, int h) {
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
//  digitalWrite(LCD_CS,LOW);/
  Address_set(x, y, x+w-1, y+h-1);
}

void TouchLCD::writePixel(int c) {
  Lcd_Write_Data(c >> 8);
  Lcd_Write_Data(c);
}

void TouchLCD::finishPixelWrite(void) {
//  digitalWrite(LCD_CS,HIGH);/
}

void TouchLCD::drawPoint(int x, int y, int c) {
  this->setupPixelWrite(x, y, x, y);
  this->writePixel(c);
  this->finishPixelWrite();
}

void TouchLCD::clear(unsigned int j) {
//  Serial.println("Clearing");
   this->setupPixelWrite(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);

  for(int i=0;i<DISPLAY_HEIGHT;i++) {
    for(int m=0;m<DISPLAY_WIDTH;m++) {
       this->writePixel(j);
    }
  }
   this->finishPixelWrite();   
}

void TouchLCD::drawVline(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i,j;
   this->Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
//  digitalWrite(LCD_CS,LOW);/
  l=l+y;
   this->Address_set(x,y,x, l);
  j=l;
  for(i=1;i<=j;i++) {
     this->Lcd_Write_Data(c >> 8);
     this->Lcd_Write_Data(c);
  }
//  digitalWrite(LCD_CS,HIGH);/   
}

void TouchLCD::drawHline(unsigned int x, unsigned int y, unsigned int l, unsigned int c) { 
  unsigned int i,j;
   this->Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
//  digitalWrite(LCD_CS,LOW);/
  l=l+x;
   this->Address_set(x,y, l, y);
  j=l;
  for(i=1;i<=j;i++) { 
    this->Lcd_Write_Data(c >> 8);
    this->Lcd_Write_Data(c);
  }
//  digitalWrite(LCD_CS,HIGH);/   
}

void TouchLCD::drawRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) {
  this->drawHline(x  , y  , w, c);
  this->drawHline(x  , y+h, w, c);
  this->drawVline(x  , y  , h, c);
  this->drawVline(x+w, y  , h, c);
}

void TouchLCD::fillRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) {
  unsigned int i;
  for(i=0;i<h;i++) {
    this->drawHline(x  , y  , w, c);
    this->drawHline(x  , y+i, w, c);
  }
}

void TouchLCD::displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH) {
//  Serial.printf("Drawing sprite: %d %d %d %d\n", xpos, ypos, xpos+(w*scaleW), ypos+(h*scaleH));
  this->setupPixelWrite(xpos, ypos, (w*scaleW), (h*scaleH));
  for(int q = 0; q < 480; q++) {
    //writePixel(GREEN);
  }
  for(int y = 0; y < h; y++) {
    for(int i = 0; i < scaleH; i++) {
      for(int x = 0; x < w; x++) {
        for(int j = 0; j < scaleW; j++) {
        
          this->writePixel(img[(y*w)+x] * RED);
          //Serial.printf("%d %d %d %d %d\n", x, y, i, j, img[y*w+x]);
//          Serial.print(img[y*w+x]);
        }
      }
//      Serial.print("\n");
    }
  }
  this->finishPixelWrite();
//  Serial.println("Done drawing sprite");
}

void TouchLCD::displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor) {
  this->setupPixelWrite(xpos, ypos, (CHAR_SIZE*scale), (CHAR_SIZE*scale));
  uint8_t* spt = &font[(0x21)*CHAR_SIZE];
  for(int y = 0; y < CHAR_SIZE; y++) {
    for(int yscale = 0; yscale < scale; yscale++) {
      int row = font[(((int) c)*CHAR_SIZE)+y];
      for(int x = 0; x < CHAR_SIZE; x++) {
        int px = ((row >> 7) & 1);
        row <<= 1;
        for(int xscale = 0; xscale < scale; xscale++) {
          this->writePixel((px) ? fontcolor : backgroundcolor);
        }
      }
    }
    spt++;
  }
  this->finishPixelWrite();
}

void TouchLCD::displayString(int xstart, int ystart, const char* str, int scale, int fontcolor, int backgroundcolor) {
  const char* c = str;
  int xpos = xstart;
  while(*c != '\0') {
    this->displayChar(xpos, ystart, *c, scale, fontcolor, backgroundcolor);
    xpos += (CHAR_SIZE*scale);
    c++;
  }
}

void TouchLCD::screenshot(void) {
  
}
