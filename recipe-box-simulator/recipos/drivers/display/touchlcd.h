#ifndef LCD_H
#define LCD_H

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define BIT_COUNT 8
/*
#define LCD_RD   32
#define LCD_WR   31     
#define LCD_RS   30        
#define LCD_CS   29       
#define LCD_REST 28
*/
//#define LCD_RD   28/
#define LCD_WR   21     // Need analog
#define LCD_RS   23     // Needs analog
//#define LCD_CS   27/       
#define LCD_REST 22

#define BIT_0 33
#define BIT_1 34
#define BIT_2 35
#define BIT_3 36
#define BIT_4 37
#define BIT_5 38
#define BIT_6 39
#define BIT_7 40

//define some color values
#define BLACK           0x0000 // 0
#define BLUE            0x0015 // 1
#define GREEN           0x0540 // 2
#define CYAN            0x0555 // 3
#define RED             0xA800 // 4
#define MAGENTA         0xA815 // 5
#define YELLOW          0xFBC1 //0xAAA0 // 6
#define LIGHT_GREY      0xAD55 // 7
#define DARK_GREY       0x52AA // 8
#define BRIGHT_BLUE     0x52BF // 9
#define BRIGHT_GREEN    0x57EA // A
#define BRIGHT_CYAN     0x57FF // B
#define BRIGHT_RED      0xFAAA // C
#define BRIGHT_MAGENTA  0xFABF // D
#define BRIGHT_YELLOW   0xFFEA // E
#define WHITE           0xFFFF // F
#define ORANGE

// LCD Bit format
// RRRRRGGGGGGBBBBB
// 1111100000011111

#include "DisplayBackend.h"

class TouchLCD : public DisplayBackend {
  public:
  // Setup
    TouchLCD(void);
  // Drawing
    int colors[16] = {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, LIGHT_GREY, DARK_GREY, BRIGHT_BLUE, BRIGHT_GREEN, BRIGHT_CYAN, BRIGHT_RED, BRIGHT_MAGENTA, BRIGHT_YELLOW, WHITE};
	int RGB(uint8_t r, uint8_t g, uint8_t b);
	char hex(int i);
	void setupPixelWrite(int x, int y, int w, int h);
	void writePixel(int c);
	void finishPixelWrite(void);
	void drawPoint(int x, int y, int c);
	void clear(unsigned int j);
	void drawVline(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
	void drawHline(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
	void drawRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
	void fillRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
	void displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH);
	void displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor);
	void displayString(int xstart, int ystart, const char* str, int scale, int fontcolor, int backgroundcolor);
	void screenshot(void); // Not implemented yet
	void sleep(void);
  private:
  // Comms
    int bits[8] = { BIT_0, BIT_1, BIT_2, BIT_3, BIT_4, BIT_5, BIT_6, BIT_7 };
    void Lcd_Writ_Bus(unsigned char d);
    void Lcd_Write_Com(unsigned char VH);
    void Lcd_Write_Data(unsigned char VH);
    void Lcd_Write_Com_Data(unsigned char com,unsigned char dat);
    void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
};

#endif
