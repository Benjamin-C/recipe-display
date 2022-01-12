/*
 * DisplayBackend.h
 *
 *  Created on: Jan 10, 2022
 *      Author: benjamin
 */

#ifndef RECIPOS_DRIVERS_DISPLAYBACKEND_H_
#define RECIPOS_DRIVERS_DISPLAYBACKEND_H_

#include <stdint.h>

class DisplayBackend {
public:
	virtual ~DisplayBackend() { };
	virtual int RGB(uint8_t r, uint8_t g, uint8_t b) = 0;
	virtual void setupPixelWrite(int x, int y, int w, int h) = 0;
	virtual void writePixel(int c) = 0;
	virtual void finishPixelWrite(void) = 0;
	virtual void drawPoint(int x, int y, int c) = 0;
    virtual void clear(unsigned int j) = 0;
    virtual void drawVline(unsigned int x, unsigned int y, unsigned int l, unsigned int c) = 0;
    virtual void drawHline(unsigned int x, unsigned int y, unsigned int l, unsigned int c) = 0;
    virtual void drawRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) = 0;
    virtual void fillRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) = 0;
    virtual void displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH) = 0;
    virtual void displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor) = 0;
    virtual void displayString(int xstart, int ystart, const char* str, int scale, int fontcolor, int backgroundcolor) = 0;
    virtual void screenshot(void) = 0; // Not implemented yet
};

#endif /* RECIPOS_DRIVERS_DISPLAYBACKEND_H_ */
