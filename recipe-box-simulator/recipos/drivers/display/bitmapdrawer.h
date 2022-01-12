#ifndef LCD_H
#define LCD_H
// This copy of the TouchLCD library is only meant for simulation.
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdint.h>

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

// LCD Bit format
// RRRRRGGGGGGBBBBB
// 1111100000011111

struct Pixel {
	uint8_t b;
	uint8_t g;
	uint8_t r;
};

#define HEADER_LENGTH 122

#include "../display/DisplayBackend.h"

class BitmapDrawer : public DisplayBackend{
  public:
  // Setup
    BitmapDrawer(void);
    ~BitmapDrawer(void);
  // Drawing
    int RGB(uint8_t r, uint8_t g, uint8_t b);
    Pixel toRGB(int c);
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
  private:
  // Comms
    std::ofstream outfile;
    const uint8_t header[HEADER_LENGTH] = {
    		0x42, 0x4D, 0x7A, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x00, 0x00, 0x00, 0x6C, 0x00,
    		0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x08, 0x07, 0x00, 0x23, 0x2E, 0x00, 0x00, 0x23, 0x2E, 0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
    int x, y, startx, starty, endx, endy;
    Pixel buff[DISPLAY_WIDTH*DISPLAY_HEIGHT];
};

#endif
