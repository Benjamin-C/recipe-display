#include "terminal.h"
#include "lcdtest.h"
#include "arduinostandin.h"
#include "recipos/drivers/display/bitmapdrawer.h"

#define PACMAN_W 7
#define PCAMAN_H 7
int pacman[PCAMAN_H*PACMAN_W] = {
  0, 0, 1, 1, 1, 0, 0,
  0, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 0, 0,
  1, 1, 1, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 0, 0,
  0, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 0, 0,
};

//TouchLCD lcd;

int h = 0;

void testText() {
  lcd.clear(BLACK);
  lcd.displayString(280, 50, "Hello world", 2, MAGENTA, BLUE);

  int xs = 10;
  int x = xs;
  int y = 10;
  int scale = 2;
  int inc = 8*scale;
  int lw = 16;
  for(int i = 0; i < 256; i++) {
    lcd.displayChar(x, y, (char) i, scale, MAGENTA, BLACK);
    if(i % lw == lw-1) {
      y += inc;
      x = xs;
    } else {
      x += inc;
    }
  }

  xs = 280;
  x = xs;
  y = 120;
  scale = 2;
  inc = 8*scale*2;
  lw = 4;
  for(int i = 0; i < 16; i++) {
    lcd.fillRect(x, y, inc, inc, lcd.colors[i]);
    lcd.displayChar(x+(inc/4), y+(inc/4), lcd.hex(i), scale, (i == 0) ? WHITE : BLACK, lcd.colors[i]);
    if(i % lw == lw-1) {
      y += inc;
      x = xs;
    } else {
      x += inc;
    }
  }
}

//void drawGradiant() {
//  lcd.setupPixelWrite(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
//
//  for(int y = 0; y < DISPLAY_HEIGHT; y++) {
//    for(int x = 0; x < DISPLAY_WIDTH; x++) {//
////      writePixel(RGB(0, 0, map(x, 0, 480, 0, 255)));/
//      lcd.writePixel(lcd.RGB(map(y, 0, DISPLAY_HEIGHT, 0, 255), 0, map(x, 0, DISPLAY_WIDTH, 0, 255)));
//    }
//  }
//  lcd.finishPixelWrite();
//}

void drawMenu() {
  lcd.clear(BLACK);

  lcd.displayString(0, 0, "This is some text for the top ", 2, BLACK, GREEN);

  for(int i = 0; i < 8; i++) {
    char text[28];
    sprintf(text, "%c This is big text", lcd.hex(i));
    lcd.displayString(28, 32 + (24*i), text, 3, WHITE, BLACK);
  }

  int ypos = 250;
  
  lcd.drawHline(0, ypos, DISPLAY_WIDTH, WHITE);

  int sep = DISPLAY_WIDTH / 5;
  for(int j = 0; j <= 4; j++) {
    if(j > 0) {
      lcd.drawVline((sep * j), ypos, (DISPLAY_HEIGHT - ypos), WHITE);
    }
    lcd.displayString((sep * j)+12, ypos+8, "menu", 2, WHITE, BLACK);
    lcd.displayString((sep * j)+12, ypos+26, "line", 2, WHITE, BLACK);
    lcd.displayString((sep * j)+12, ypos+42, "three", 2, WHITE, BLACK);
  }
}

TextTerminal terminal(&lcd);

void drawTerminal() {
  terminal.reset();
  Serial.println("Drawing text");
  terminal.setScroll(true);
  terminal.clear();
  terminal.setCursor(0, 5);
  terminal.print_c("This is a message that should be much too long to fit on one line or maybe even on three lines", EGA_WHITE, EGA_BLACK);
  terminal.newline();
  terminal.newline();
  terminal.newline();
  terminal.println("This is line 1");
  terminal.println("This is line 2");
  terminal.newline();
  terminal.printChar_c(0xB0, EGA_BRIGHT_CYAN, EGA_BLACK);
  terminal.printChar_c(0x01, EGA_BRIGHT_GREEN, EGA_BLACK);
  terminal.printChar_c(0x0F, EGA_BRIGHT_YELLOW, EGA_BLACK);
  terminal.newline();
  terminal.newline();
  terminal.render();
}

void textMenu() {
  terminal.reset();
  terminal.print_c("This is some top text         ", EGA_BLACK, EGA_YELLOW);

  terminal.print("In computing, a serial port is"\
                 "a serial communication inter- "\
                 "face through which information"\
                 "transfers in or   out sequen- "\
                 "tially one bit at a time. This"\
                 "is in contrast to a parallel  "\
                 "port, which communicates      "\
                 "multiple bits simultaneously  "\
                 "in parallel. Throughout most  "\
                 "of the history of personal    "\
                 "omputers, data has been trans-"\
                 "ferred through serial ports to"\
                 "devices such as modems, term- "\
                 "inals, various peripherals,   "\
                 "and directly between computers");
  
  terminal.setCursor(0, 16);
  for(int i = 0; i < 5; i++) {
    if(i != 0) {
      terminal.printChar_c(0xC2, EGA_LIGHT_GREY, EGA_BLACK);
    }
    for(int j = 0; j < 5; j++) {
      terminal.printChar_c(0xC4, EGA_LIGHT_GREY, EGA_BLACK);
    }
    if(i == 2) {
      terminal.printChar_c(0xC4, EGA_LIGHT_GREY, EGA_BLACK);
    }
  }
  for(int k = 0; k < 3; k++) {
    for(int i = 0; i < 5; i++) {
      if(i != 0) {
        terminal.printChar_c(0xB3, EGA_LIGHT_GREY, EGA_BLACK);
      }
      switch(i) {
        case 0: terminal.setColor(EGA_BRIGHT_RED); break;
        case 1: terminal.setColor(EGA_BRIGHT_YELLOW); break;
        case 2: terminal.setColor(EGA_BRIGHT_GREEN); break;
        case 3: terminal.setColor(EGA_BRIGHT_BLUE); break;
        case 4: terminal.setColor(EGA_BRIGHT_MAGENTA); break;
      }
      switch(k) {
        case 0: terminal.print("I has"); break;
        case 1: terminal.print("Three"); break;
        case 2: terminal.print("Lines"); break;
      }
      if(i == 2) {
        terminal.printChar(0x00);
      }
    }
  }
  terminal.render();
}

unsigned long nextFlash = 0;

void runDemo() {
  // The terminal showing scrolling
  terminal.reset();
/*
  drawTerminal();
  int number = 0;
  for(int i = 0; i < 20; i++) {
    char line[MAX_TEXT_LINE_LENGTH];
    sprintf(line, "Text Line %d", number++);
    terminal.println(line);
    terminal.render();
//    lcd.screenshot();
//    delay(500);
    delay(50);
  }
  lcd.screenshot();
*/

  // Basic text drawing demo
  testText();
  lcd.screenshot();
  delay(5000);

/*
  // Draw lots of boxes
  unsigned long endms = millis() + 5000;
//  drawGradiant();
  lcd.screenshot();
  while(endms > millis()) {
    int x = random(DISPLAY_WIDTH);
    int y = random(DISPLAY_HEIGHT);
    int w = random(DISPLAY_WIDTH - x);
    int h = random(DISPLAY_HEIGHT - y);
    lcd.drawRect(x,y,w,h,lcd.RGB(random(255), random(255), random(255))); // rectangle at x, y, with, hight, color
    delay(1);
  }
*/

  // Testing the menu as graphics
  drawMenu();
  lcd.screenshot();
  delay(5000);


  // Solid colors
  for(int i = 0; i < 5; i++) {
    lcd.clear(0xf800);
    lcd.screenshot();
    delay(333);
    lcd.clear(0x07E0);
    lcd.screenshot();
    delay(333);
    lcd.clear(0x001F);
    lcd.screenshot();
    delay(333);
  }


  // Testing the menu as text
  textMenu();
  lcd.screenshot();
  delay(5000);

/*
  terminal.clear();
  nextFlash = millis();
*/
/*
  terminal.reset();
  demoScreen();
*/
}

void demoScreen() {
  terminal.clear();
  terminal.printASCII("Welcome to Ben's terminal demo\33[2E"\
                    "Characters sent over serial\33[E"\
                    "  will be displayed on this\33[E"\
                    "  screen. Use escape sequences\33[E"\
                    "  for special fetures.\33[E"\
                    "To use an escape sequence, use\33[E"\
                    "  \33[95m<Esc>\33[94m[\33[92mn\33[91mx\33[39m where \33[92mn\33[39m is a number"\
                    "  and \33[91mx\33[39m is a letter from below"\
                    "  \33[92mn\33[39m is DEC, optional, def. 1\33[E"\
                    " \33[91mA\33[39m,\33[91mB\33[39m,\33[91mC\33[39m,\33[91mD\33[39m move cursor \33[24s\33[25s\33[26s\33[27s by \33[92mn\33[39m"\
                    " \33[91mE\33[39m,\33[91mF\33[39m move cursor to start of \33[E    line \33[92mn\33[39m \33[25s\33[24s\33[E"\
                    " \33[92m2\33[91mJ\33[39m clear the screen\33[E"\
                    " \33[91mz\33[39m run demo\33[E"\
                    " \33[91ms\33[39m display char \33[92mn\33[39m from cp437\33[E"\
                    " \33[91mm\33[39m set color to \33[92mn\33[39m\33[E"\
                    "Text\33[16s 3x:\33[107m\33[30m0\33[49m\33[31m1\33[32m2\33[33m3\33[34m4\33[35m5\33[36m6\33[37m7\33[39m  9x:\33[90m0\33[91m1\33[92m2\33[93m3\33[94m4\33[95m5\33[96m6\33[97m7\33[E"\
                    "Bkg \33[16s 4x:\33[40m0\33[41m1\33[42m2\33[43m3\33[44m4\33[45m5\33[46m6\33[47m7\33[49m 10x:\33[100m0\33[101m1\33[102m2\33[103m3\33[104m4\33[105m5\33[106m6\33[30m\33[107m7\33[39m\33[49m\33[E");
  terminal.render();
}

void setup() {
  lcd.clear(BLACK);
  terminal.foo = &runDemo;
//  demoScreen();

/*  terminal.printChar_c('x', EGA_BRIGHT_RED, EGA_BLACK);
  terminal.println(" is a letter from below");
  terminal.newline();
  terminal.print_c(" A,B,C,D", EGA_BRIGHT_GREEN, EGA_BLACK);
  terminal.println(" to move the cursor");*/
}

void loop() {
  if(Serial.available()) {
    while(Serial.available()) {
      terminal.asciiChar((uint8_t) Serial.read());
    }
    terminal.render();
  }

  while(nextFlash < millis()) {
    nextFlash += 500;
    bool next = !digitalRead(LED_BUILTIN);
    terminal.showCursor = next;
    terminal.render();
    digitalWrite(LED_BUILTIN, next);
  }
}
