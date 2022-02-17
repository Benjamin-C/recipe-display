/* display.h
 *
 * Display module for RecipOS
 * Uses the display driver to handle the display hardware
 *
 * Author: Benjamin Crall
 */

#ifndef RECIPOS_MODULES_DISPLAY_H_
#define RECIPOS_MODULES_DISPLAY_H_

#include <stdint.h>
#include <string>

#include "../drivers/display/DisplayBackend.h"

// All X,Y coordinates used in this module are relative to the top left corner of the allowed drawing window.
// Any draw command that will go out of bounds will be rejected.
// The draw commands all return weather or not they were allowed to proceed.
//   This does not guarantee that the draw actually worked.

// This system uses 16 bit color.
// The color order is: RRRRRGGGGGGBBBBB
// +-------+---------+---------+---------+
// | Color | Red     | Green   | Blue    |
// +-------+---------+---------+---------+
// | Count |  5      |  6      |  5      |
// | Range | [15:11] | [10:5]  | [4:0]   |
// +-------+---------+---------+---------+

#include "../colors.h"

class Display {
public:
	// The listed colors in the order of EGA colors
	const int colors[16] = {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, LIGHT_GREY, DARK_GREY, BRIGHT_BLUE, BRIGHT_GREEN, BRIGHT_CYAN, BRIGHT_RED, BRIGHT_MAGENTA, BRIGHT_YELLOW, WHITE};
	// init
	Display(DisplayBackend* dbe, int xmin, int ymin, int xmax, int ymax);
	// Convert 24bit color to 16bit in the format needed for this module
	int RGB(uint8_t r, uint8_t g, uint8_t b);
	// Converts a number to a single hex character
	char hex(int i);
	// Draws a point at x,y, with a color c
	bool drawPoint(int x, int y, int c);
	// Fills the drawing area with color c
    bool fill(int c);
    // Draws a vertical line starting at x,y with the length l and the color c
    bool drawVline(int x, int y, int l, int c);
    // Draws a horizontal line starting at x,y with the length l and the color c
    bool drawHline(int x, int y, int l, int c);
    // Outlines a rectangle starting at x,y with size w,h and color c
    bool drawRect(int x,int y,int w,int h,int c);
    // Fills a rectangle starting at x,y with size w,h and color c
    bool fillRect(int x, int y,int w,int h,int c);
    // Displays a sprite at xpos,ypos. See display.cpp for more info
    // Don't use this for now, it will change
    // TODO fix sprite drawing
    bool displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH);
    // Displays character c at xpos,ypos with a scale scale and color fontcolor on color backgroundcolor
    bool displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor);
    // Displays string str at xpos,ypos with a scale scale and color fontcolor on color backgroundcolor
    bool displayString(int xstart, int ystart, std::string str, int scale, int fontcolor, int backgroundcolor);
    // Asks the underlying display driver to take a screenshot. Exactly what this does depends on that driver.
    bool screenshot(void);
    // Get the width of the allowed drawing window
    int width(void);
    // Gets the height of the allowed drawing window
    int height(void);
    // Gets weather or not the display is enabled.
    bool isEnabled(void);
    // Sets wether the display is enabled.
    // DO NOT DO THIS unless you are sure you need to, it could break things if done at the wrong time.
    // TODO see if this should be a friend method
    void setEnabled(bool en);
private:
    DisplayBackend* dbe;
    bool inBounds(int x, int y, int w, int h);
    int xmin;
    int xmax;
    int ymin;
    int ymax;
    bool enabled = false;
};

#endif /* RECIPOS_MODULES_DISPLAY_H_ */
