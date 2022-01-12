#ifndef RECIPOS_MODULES_DISPLAY_H_
#define RECIPOS_MODULES_DISPLAY_H_

#include <stdint.h>

#include "../drivers/display/DisplayBackend.h"

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

class Display {
public:
	const int colors[16] = {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, LIGHT_GREY, DARK_GREY, BRIGHT_BLUE, BRIGHT_GREEN, BRIGHT_CYAN, BRIGHT_RED, BRIGHT_MAGENTA, BRIGHT_YELLOW, WHITE};
	Display(DisplayBackend* dbe, int xmin, int ymin, int xmax, int ymax);
	int RGB(uint8_t r, uint8_t g, uint8_t b);
	char hex(int i);
	bool drawPoint(int x, int y, int c);
    bool clear(int j);
    bool drawVline(int x, int y, int l, int c);
    bool drawHline(int x, int y, int l, int c);
    bool drawRect(int x,int y,int w,int h,int c);
    bool fillRect(int x, int y,int w,int h,int c);
    bool displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH);
    bool displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor);
    bool displayString(int xstart, int ystart, const char* str, int scale, int fontcolor, int backgroundcolor);
    bool screenshot(void); // Not implemented yet
    int width(void);
    int height(void);
private:
    DisplayBackend* dbe;
    bool inBounds(int x, int y, int w, int h);
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};


#endif /* RECIPOS_MODULES_DISPLAY_H_ */
