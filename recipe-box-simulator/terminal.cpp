#include "terminal.h"

#include "arduinostandin.h"
#include <stdbool.h>

#include "recipos/modules/Display.h"

//  public:
//    TextTerminal(TouchLCD screen);
//    void print(const char* msg);
//    void print_c(const char* msg, int fore, int back);
//    void println(const char* msg);
//    void println_c(const char* msg, int fore, int back);
//    void printChar(uint8_t c);
//    void printChar_c(uint8_t c, int fore, int back);
//    void bufferChar(uint8_t c, int fore, int back);
//    void newline(void);
//    void clear(void);
//    void setColor(int color);
//    void setBackground(int color);
//    void setCursor(int x, int y);
//    void setScroll(bool scroll);
//    void render(void);
//    TouchLCD screen;
//  private:
//    uint16_t* buffer[MAX_TEXT_LINES]; //[15:12] Background color [11:8] Text color [7:0] character
//    int scale;
//    int width;
//    int height;
//    bool scroll;
//    bool error;
//    int cursorX;
//    int curosrY;
//    void incrementCursor(void);

TextTerminal::TextTerminal(Display* screen) {
  this->screen = screen;
  this->scale = TEXT_TERMINAL_SCALE;
  this->width = (DISPLAY_WIDTH/CHAR_SIZE)/TEXT_TERMINAL_SCALE;
  this->height = (DISPLAY_HEIGHT/CHAR_SIZE)/TEXT_TERMINAL_SCALE;
  this->scroll = true;
  this->error = false;
  this->cursorX = 0;
  this->cursorY = 0;
  currentForeground = DEFAULT_FOREGROUND;
  currentBackground = DEFAULT_BACKGORUND;
  escape = 0;
  showCursor = false;
  escapeN = 0;
  foo = NULL;


  for(int i = 0; i < this->height; i++) {
    uint16_t* ptr = (uint16_t*) malloc(sizeof(uint16_t) * this->width);
    this->buffer[i] = ptr;
    if(ptr == NULL) {
      this->error = true;
      Serial.printf("Could not initialize buffer line %d\n", i);
    }
  }
}

TextTerminal::~TextTerminal() {
  for(int i = 0; i < this->height; i++) {
    free(this->buffer[i]);
  }
}

void TextTerminal::print(const char* msg) {
  this->print_c(msg, currentForeground, currentBackground);
}

void TextTerminal::print_c(const char* msg, int fore, int back) {
  const char* c = msg;
  while(*c != '\0') {
    this->bufferChar(*c, fore, back);
    cursorX++;
    c++;
  }
}

void TextTerminal::println(const char* msg) {
  println_c(msg, currentForeground, currentBackground);
}

void TextTerminal::println_c(const char* msg, int fore, int back) {
  print_c(msg, fore, back);
  newline();
}

void TextTerminal::printChar(uint8_t c) {
  bufferChar(c, currentForeground, currentBackground);
  cursorX++;
}

void TextTerminal::printChar_c(uint8_t c, int fore, int back) {
  bufferChar(c, fore, back);
  cursorX++;
}

void TextTerminal::asciiChar(uint8_t c) {
  asciiChar_c(c, currentForeground, currentBackground);
}

void TextTerminal::asciiChar_c(uint8_t c, int fore, int back) {
  if(c == '\n') {
    newline();
  } else {
    if(escape == 0) {
      if(c == 0x08) { // backspace
        moveCursor(-1, 0);
      } else if(c == 0x1B) { // escape
        escape = 1;
      } else if(c != '\r') {
        printChar(c);
      }
    } else {
      switch(escape) {
        case 1: {
          if(c == '[') {
            escape = 2;
            escapeN = 0;
          } else {
            printChar(c);
            escape = 0;
          }
        } break;
        default: {
          if(c >= 0x40 && c <= 0x7E) {
            escape = 0;
            switch(c) {
              case 'A': moveCursor(0, (escapeN > 0) ? -escapeN : -1); break;
              case 'B': moveCursor(0, (escapeN > 0) ? escapeN : 1); break;
              case 'C': moveCursor((escapeN > 0) ? escapeN : 1, 0); break;
              case 'D': moveCursor((escapeN > 0) ? -escapeN : -1, 0); break;
              case 'E': cursorX = 0; cursorY += (escapeN > 0) ? escapeN : 1; break;
              case 'F': cursorX = 0; cursorY -= (escapeN > 0) ? escapeN : 1; break;
              case 'm': {
                if(escapeN >= 30 && escapeN <= 37) {
                  currentForeground = asciiColors[escapeN - 30];
                } else if(escapeN >= 40 && escapeN <= 47) {
                  currentBackground = asciiColors[escapeN - 40];
                } else if(escapeN >= 90 && escapeN <= 97) {
                  currentForeground = asciiColors[escapeN - 82];
                } else if(escapeN >= 100 && escapeN <= 107) {
                  currentBackground = asciiColors[escapeN - 92];
                } else if(escapeN == 39) {
                  currentForeground = DEFAULT_FOREGROUND;
                } else if(escapeN == 49) {
                  currentBackground = DEFAULT_BACKGORUND;
                }
              } break;
              case 'z': foo(); break; // Custom
              case 's': printChar((uint8_t) (escapeN & 0xFF)); break; // custom
              case 'J': {
                switch(escapeN) {
                  case 0: {
                    // Not implemented, should clear from cursor to end of screen
                  } break;
                  case 1: {
                    // Not implemented, should clear from cursor to beginning of screen
                  } break;
                  case 2:
                  case 3: {// Equivelent on this system
                    clear();
                  } break;
                }
              } break;
              default: printChar_c(c, EGA_WHITE, EGA_BRIGHT_RED); printChar_c(escapeN, EGA_BLUE, EGA_BRIGHT_GREEN); break;
            }
          } else if(c >= 0x30 && c <= 0x39) {
            escapeN *= 10;
            escapeN += c - 0x30;
          }
        } break;
      }
    }
  }
}

void TextTerminal::printASCII(const char* msg) {
  for(char* c = (char*) msg; *c != '\0'; c++) {
     asciiChar(*c);
  }
}

void TextTerminal::bufferChar(uint8_t c, int fore, int back) {
  if(cursorX < 0) {
    cursorX = 0;
  }
  if(cursorY < 0) {
    cursorY = 0;
  }
  if(cursorX >= this->width) {
    cursorY++;
    cursorX = 0;
  }
  scrollIfNeeded();
  uint16_t* buffline = this->buffer[cursorY];
  if(buffline != NULL) {
    buffline[cursorX] = (c & 0xFF) | ((fore & 0xF) << 8) | ((back & 0xF) << 12);
  } else {
    Serial.printf("Could not buffer char at %d,%d, line was null", cursorX, cursorY);
    delay(1);
  }
}

void TextTerminal::newline(void) {
  cursorY++;
  cursorX = 0;
  scrollIfNeeded_m(this->height+1);
}

void TextTerminal::reset(void) {
  clear();
  currentForeground = DEFAULT_FOREGROUND;
  currentBackground = DEFAULT_BACKGORUND;
  showCursor = false;
}

void TextTerminal::clear(void) {
  for(int i = 0; i < this->height; i++) {
    clearLine(i);
  }
  cursorX = 0;
  cursorY = 0;
}

void TextTerminal::clearLine(int line) {
  uint16_t* c = this->buffer[line];
  if(c == NULL) {
    Serial.printf("Could not clear line %d, it was null", line);
  } else {
    for(int j = 0; j < this->width; j++) {
      *c = 0x0F00;
      c++;
    }
  }
}

void TextTerminal::setColor(int color) {
  currentForeground = color;
}

void TextTerminal::setBackground(int color) {
  currentBackground = color;
}

void TextTerminal::setCursor(int x, int y) {
  cursorX = x;
  cursorY = y;
}

void TextTerminal::moveCursor(int x, int y) {
  cursorX += x;
  cursorY += y;
}

void TextTerminal::setScroll(bool scroll) {
  this->scroll = scroll;
}

void TextTerminal::scrollIfNeeded(void) {
  scrollIfNeeded_m(this->height);
}

void TextTerminal::scrollIfNeeded_m(int max) {
  while(cursorY >= max) {
    if(scroll) {
      scrollUp();
      cursorY--;
      clearLine(this->height-1);
    } else {
      cursorY = 0;
    }
  }
}

void TextTerminal::scrollUp(void) {
  uint16_t* line = buffer[0];
  for(int i = 0; i < this->height-1; i++) {
    buffer[i] = buffer[i+1];
  }
  buffer[this->height-1] = line;
}

void TextTerminal::scrollDown(void) {
  uint16_t* line = buffer[this->height-1];
  for(int i = this->height-1; i > 0; i--) {
    buffer[i] = buffer[i-1];
  }
  buffer[0] = line;
}

void TextTerminal::render(void) {
//  unsigned long start = micros();
  for(int i = 0; i < this->height; i++) {
    uint16_t* c = this->buffer[i];
    if(c != NULL) {
      int xpos = 0;
      for(int j = 0; j < this->width; j++) {
        
        char ch = *c;
        int fore = screen->colors[(*c >> 8) & 0x0F];
        int back = screen->colors[(*c >> 12) & 0x0F];
        if(showCursor) {
          if(i == cursorY && j == cursorX) {
            if(ch == 0) {
              ch = '_';
              fore = screen->colors[currentForeground];
              back = screen->colors[currentBackground];
            } else {
              int tmp = fore;
              fore = back;
              back = tmp;
            }
          }
        }
        screen->displayChar(xpos, i*CHAR_SIZE*scale, (const char) (ch & 0xFF), scale, fore, back);
        
        //screen.displayChar(xpos, i*CHAR_SIZE*scale, (const char) (*c & 0xFF), scale, screen.colors[(*c >> 8) & 0x0F], screen.colors[(*c >> 12) & 0x0F]);
        xpos += (CHAR_SIZE*scale);
        c++;
      }
    } else {
      Serial.printf("Could not print line %d, it was null\n", i);
    }
  }
//  unsigned long end = micros();

  //Serial.printf("Render took %duS\n", (end-start));
}
