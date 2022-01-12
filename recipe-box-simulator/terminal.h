#ifndef TERMINAL_H
#define TERMINAL_H

#include "recipos/drivers/display/bitmapdrawer.h"
#include "recipos/drivers/display/font.h"
#include "recipos/modules/Display.h"

#define TEXT_TERMINAL_SCALE 2 // Minimum scale. Higher scales allowed, but will not save memory
#define MAX_TEXT_LINES (DISPLAY_HEIGHT/CHAR_SIZE)/TEXT_TERMINAL_SCALE
#define MAX_TEXT_LINE_LENGTH (DISPLAY_WIDTH/CHAR_SIZE)/TEXT_TERMINAL_SCALE

#define EGA_BLACK           0x0
#define EGA_BLUE            0x1
#define EGA_GREEN           0x2
#define EGA_CYAN            0x3
#define EGA_RED             0x4
#define EGA_MAGENTA         0x5
#define EGA_YELLOW          0x6
#define EGA_LIGHT_GREY      0x7
#define EGA_DARK_GREY       0x8
#define EGA_BRIGHT_BLUE     0x9
#define EGA_BRIGHT_GREEN    0xA
#define EGA_BRIGHT_CYAN     0xB
#define EGA_BRIGHT_RED      0xC
#define EGA_BRIGHT_MAGENTA  0xD
#define EGA_BRIGHT_YELLOW   0xE
#define EGA_WHITE           0xF

#define DEFAULT_FOREGROUND EGA_WHITE
#define DEFAULT_BACKGORUND EGA_BLACK

class TextTerminal {
  public:
    TextTerminal(Display* screen);
    ~TextTerminal();
    void print(const char* msg);
    void print_c(const char* msg, int fore, int back);
    void println(const char* msg);
    void println_c(const char* msg, int fore, int back);
    void printChar(uint8_t c);
    void printChar_c(uint8_t c, int fore, int back);
    void asciiChar(uint8_t c);
    void asciiChar_c(uint8_t c, int fore, int back);
    void printASCII(const char* msg);
    void bufferChar(uint8_t c, int fore, int back);
    void newline(void);
    void reset(void);
    void clear(void);
    void clearLine(int line);
    void setColor(int color);
    void setBackground(int color);
    void setCursor(int x, int y);
    void moveCursor(int x, int y);
    void setScroll(bool scroll);
    void scrollIfNeeded(void);
    void scrollIfNeeded_m(int max);
    void scrollUp(void);
    void scrollDown(void);
    void render(void);
    Display* screen;
    bool showCursor;
    int width;
    int height;
    void (*foo)(void);
  private:
    uint16_t* buffer[MAX_TEXT_LINES]; //[15:12] Background color [11:8] Text color [7:0] character
    int scale;
    bool scroll;
    bool error;
    int cursorX;
    int cursorY;
    int currentForeground;
    int currentBackground;
    int escape;
    int escapeN;
    uint8_t asciiColors[16] = {EGA_BLACK, EGA_RED, EGA_GREEN, EGA_YELLOW, EGA_BLUE, EGA_MAGENTA, EGA_CYAN, EGA_LIGHT_GREY, EGA_DARK_GREY, EGA_BRIGHT_RED, EGA_BRIGHT_GREEN,  EGA_BRIGHT_YELLOW, EGA_BRIGHT_BLUE, EGA_BRIGHT_MAGENTA, EGA_BRIGHT_CYAN, EGA_WHITE};
};

#endif
