// TODO add comments

/* display.cpp
 *
 * Display module for RecipOS
 * Uses the display driver to handle the display hardware
 *
 * Author: Benjamin Crall
 */

#include "../drivers/display/DisplayBackend.h"
#include <string.h>
#include <stdio.h>
#include "Display.h"

//#define MEGA_DEBUG_LOG

Display::Display(DisplayBackend* dbe, int xmin, int ymin, int xmax, int ymax) {
	this->dbe = dbe;
	this->xmin = xmin;
	this->xmax = xmax;
	this->ymin = ymin;
	this->ymax = ymax;
	this->enabled = false;
	printf("Bounds are %d < x < %d & %d < y < %d? \n", xmin, xmax, ymin, ymax);
}

int Display::width(void) {
	return xmax - xmin;
}

int Display::height(void) {
	return ymax - ymin;
}

// private
bool Display::inBounds(int x, int y, int w, int h) {
	if(enabled) {
		if(x >= 0 && y >= 0 && x < xmax && y < ymax) {
			if(xmin+x+w <= xmax && ymin+y+h <= ymax) {
				return true;
			} else {
				printf("[Display] ERR: Is %d,%d (%dx%d) within %d < x < %d & %d < %d? ", x, y, w, h, xmin, xmax, ymin, ymax);
				printf("No! Too big\n");
				return false;
			}
		} else {
			printf("[Display] ERR: Is %d,%d (%dx%d) within %d < x < %d & %d < %d? ", x, y, w, h, xmin, xmax, ymin, ymax);
			printf("No! Out of bounds\n");
			return false;
		}
	} else {
		printf("Drawing is disabled for this drawer\n");
		return false;
	}
}

char Display::hex(int i) {
  i = i % 16;
  return (char) ((i < 10) ? (i+'0') : (i-10+'A'));
}

int Display::RGB(uint8_t r, uint8_t g, uint8_t b) {
	return dbe->RGB(r, g, b);
}

bool Display::drawPoint(int x, int y, int c) {
	if(inBounds(x,y,1,1)) {
		dbe->drawPoint(x+xmin, y+ymin, c);
		return true;
	} else {
		return false;
	}
}

bool Display::fill(int c) {
#ifdef MEGA_DEBUG_LOG
	printf("clear(%d) %d %d\n", c, width(), height());
#endif
	if(enabled) {
		fillRect(0, 0, width(), height(), c);
		return true;
	} else {
		printf("Drawing is disabled\n");
		return false;
	}
}
bool Display::drawVline(int x, int y, int l, int c) {
#ifdef MEGA_DEBUG_LOG
	printf("drawVline(%d, %d, %d, %d)\n", x, y, l, c);
#endif
	if(inBounds(x,y,1,l)) {
		dbe->drawVline(x+xmin, y+ymin, l, c);
		return true;
	} else {
		return false;
	}
}
bool Display::drawHline(int x, int y, int l, int c) {
#ifdef MEGA_DEBUG_LOG
	printf("drawHline(%d, %d, %d, %d)\n", x, y, l, c);
#endif
	if(inBounds(x,y,l,1)) {
		dbe->drawHline(x+xmin, y+ymin, l, c);
		return true;
	} else {
		return false;
	}
}
bool Display::drawRect(int x,int y,int w,int h,int c) {
#ifdef MEGA_DEBUG_LOG
	printf("drawRect(%d, %d, %d, %d, %d)\n", x, y, w, h, c);
#endif
	if(inBounds(x,y,w,h)) {
		dbe->drawRect(x+xmin, y+ymin, w, h, c);
		return true;
	} else {
		return false;
	}
}
bool Display::fillRect(int x,int y,int w,int h,int c) {
#ifdef MEGA_DEBUG_LOG
	printf("fillRect(%d, %d, %d, %d, %d)\n", x, y, w, h, c);
#endif
	if(inBounds(x,y,w,h)) {
		dbe->fillRect(x+xmin, y+ymin, w, h, c);
		return true;
	} else {
		return false;
	}
}

/* DisplaySprite()
 *
 * Don't use this, it will change.
 * I don't like how much memory it uses right now
 */
// TODO fix sprite drawing
bool Display::displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH) {
#ifdef MEGA_DEBUG_LOG
	printf("displaySprite(%d, %d, x, %d, %d, %d, %d)\n", xpos, ypos, w, h, scaleW, scaleH);
#endif
	if(inBounds(xpos, ypos,w*scaleW,h*scaleW)) {
		dbe->displaySprite(xpos+xmin,ypos+ymin, img, w, h, scaleW, scaleH);
		return true;
	} else {
		return false;
	}
}
// TODO update char size to actually work
bool Display::displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor) {
#ifdef MEGA_DEBUG_LOG
	printf("displayChar(%d, %d, %02X, %d, %d, %d)\n", xpos, ypos, c, scale, fontcolor, backgroundcolor);
#endif
	if(inBounds(xpos, ypos, 8*scale, 8*scale)) {

		dbe->displayChar(xpos+xmin,ypos+ymin, c, scale, fontcolor, backgroundcolor);
		return true;
	} else {
		return false;
	}
}
bool Display::displayString(int xstart, int ystart, const char* str, int scale, int fontcolor, int backgroundcolor) {
#ifdef MEGA_DEBUG_LOG
	printf("displayString(%d, %d, \"%s\", %d, %d, %d)\n", xstart, ystart, str, scale, fontcolor, backgroundcolor);
#endif
	if(str != NULL) {
#ifdef MEGA_DEBUG_LOG
		printf("datas: %d %d %d %d %d %d\n", xstart, ystart, (int) (8*strlen(str)*scale), 8*scale, (int) (xstart+xmin+(8*strlen(str)*scale)), ystart+ymin+(8*scale));
#endif
		if(inBounds(xstart, ystart,8*strlen(str)*scale,8*scale)) {
			dbe->displayString(xstart+xmin,ystart+ymin, str, scale, fontcolor, backgroundcolor);
			return true;
		} else {
			return false;
		}
	} else {
		printf("Nope, string was null\n");
		return false;
	}
}
bool Display::screenshot(void) {
	dbe->screenshot();
	return true;
}

bool Display::isEnabled(void) {
	return enabled;
}

void Display::setEnabled(bool en) {
	enabled = en;
}

