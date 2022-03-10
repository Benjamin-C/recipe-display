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
#include <string>
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
bool Display::displayString(int xstart, int ystart, std::string str, int scale, int fontcolor, int backgroundcolor) {
#ifdef MEGA_DEBUG_LOG
	printf("displayString(%d, %d, \"%s\", %d, %d, %d)\n", xstart, ystart, str, scale, fontcolor, backgroundcolor);
#endif
	if(!str.empty()) {
#ifdef MEGA_DEBUG_LOG
		printf("datas: %d %d %d %d %d %d\n", xstart, ystart, (int) (8*strlen(str)*scale), 8*scale, (int) (xstart+xmin+(8*strlen(str)*scale)), ystart+ymin+(8*scale));
#endif
		const char* cstr = str.c_str();
		if(inBounds(xstart, ystart,8*strlen(cstr)*scale,8*scale)) {
			dbe->displayString(xstart+xmin,ystart+ymin, cstr, scale, fontcolor, backgroundcolor);
			return true;
		} else {
			return false;
		}
	} else {
		printf("Nope, string was null\n");
		return false;
	}
}

/* Displays a string with line wrapping. Does not autowrap if the string would go off the screen.
 * May not prevent all printing if some text would be out of bounds. Hopefully won't print out of bounds ...
 * Returns the number of rows printed. Should probably return an error state, but doesn't
 *
 * int xstart - The x staritng position
 * int ystart - The y starting position
 * std::string text - The text to print
 * int width - The number of characters wide to print
 * int height - The required height to print.
 *   Set to 0 to only print as many lines as needed,
 *   Setting to > 0 will print exactly that many lines
 * int scale - The text scale
 * int fontcolor - The font color 16bit color number
 * int bkgcoolor - the background color 16bit number
 */
int Display::displayWrappedString(int xstart, int ystart, std::string text, int width, int height, int scale, int fontcolor, int bkgcolor) {
	// Get the string as a character array so we can do pointer manipulation
	const char* c = text.c_str();
	bool hasMoreMsg = true; // Is there more message to print?
	int rows = 0; // How many rows have been printed
	int inc = 8*scale; // The distance to move per character
	while((height > 0 && rows < height) || (height <= 0 && hasMoreMsg)) { // Always does 12 rows. Is this good? I don't know
		int xpos = xstart; // Reset the x position for each row
		int charCount = 0; // Number of characters already printed
		int currentLineLength = 0; // The current confirmed line length
		int tempLineLength = 0; // The temporary line length that may be confrimed later
		const char* tempChar = c; // Temporary pointer set to the beginning of the current line
		while(tempLineLength <= width) {
			// Find the next possible place to break the line
			while(*tempChar != ' ' && *tempChar != '\0' && *tempChar != '\n') {
				tempChar++; tempLineLength++;
			}
			// If that is an allowable width for the line
			if(tempLineLength <= width) {
				// Note that length
				currentLineLength = tempLineLength;
				if(*tempChar != ' ') { // If the character should start a new line, do so
					break;
				} else { // Move to the next character so loop above will pass
					tempChar++;
					tempLineLength++;
				}
			} else {
				// Maybe should do something here? I don't know
//				if(*tempChar == '\0') {
//					break;
//				}
			}
		}
		// Print the line with the correct number of characters, then print spaces to pad the line
		while(charCount < width) {
			// If the next character is printable, and fits on the line,
			if(*c != '\0' && *c != '\n' && currentLineLength > 0) {
				// Print it
				displayChar(xpos, ystart, *c, scale, fontcolor, bkgcolor);
//				printf("%c", *c);
				c++;
				currentLineLength--;
				if(currentLineLength == 0 && *c != '\0') {
					// Remove extra spaces or line breaks
					c++;
				}
			} else {
				// Otherwise, pad the line with spaces
				if(*c == '\0') {
					// Maybe stop after this line if we have reached the end of the string
					hasMoreMsg = false;
				}
				displayChar(xpos, ystart, ' ', scale, fontcolor, bkgcolor);
			}
			// Keep track of where we are
			xpos += inc;
			charCount++;
		}
		// Put new lines on new lines
		if(*c == '\n') {
			c++;
		}
		ystart += inc;
//		if(hasMoreMsg) {
//			printf("\n");
//		}
		rows++;
	}
	return rows;
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

