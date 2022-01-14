#include "bitmapdrawer.h"

#include "../display/font.h"

/*
 * ███████ ███████ ████████ ██    ██ ██████  
 * ██      ██         ██    ██    ██ ██   ██ 
 * ███████ █████      ██    ██    ██ ██████  
 *      ██ ██         ██    ██    ██ ██      
 * ███████ ███████    ██     ██████  ██      
 */
BitmapDrawer::BitmapDrawer(void) {
  startx = 0;
  starty = 0;
  endx = 0;
  endy = 0;
  x = 0;
  y = 0;
  clear(RGB(0x00, 0x00, 0x00));
  width = DISPLAY_WIDTH;
  height = DISPLAY_HEIGHT;
}

BitmapDrawer::~BitmapDrawer(void) {

}

/*
 *  ██████  ██████  ███    ███ ███    ███ ███████ 
 * ██      ██    ██ ████  ████ ████  ████ ██      
 * ██      ██    ██ ██ ████ ██ ██ ████ ██ ███████ 
 * ██      ██    ██ ██  ██  ██ ██  ██  ██      ██ 
 *  ██████  ██████  ██      ██ ██      ██ ███████ 
 */

void BitmapDrawer::Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) {
	startx = x1;
	starty = y1;
	endx = x2;
	endy = y2;
	x = startx;
	y = starty;
}

// something new

/*
 * ██████  ██████   █████  ██     ██ 
 * ██   ██ ██   ██ ██   ██ ██     ██ 
 * ██   ██ ██████  ███████ ██  █  ██ 
 * ██   ██ ██   ██ ██   ██ ██ ███ ██ 
 * ██████  ██   ██ ██   ██  ███ ███  
 */

int BitmapDrawer::RGB(uint8_t r, uint8_t g, uint8_t b) {
  return (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
}

Pixel BitmapDrawer::toRGB(int c) {
	//return (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);
	Pixel p;
	p.r = ((c >> 11) & 0x1F) << 3;
	p.g = ((c >> 5 ) & 0x3F) << 2;
	p.b = ((c >> 0 ) & 0x1F) << 3;
	return p;
}

// Call this first, then call writePixel for each pixel
void BitmapDrawer::setupPixelWrite(int x, int y, int w, int h) {
  Address_set(x, y, x+w-1, y+h-1);
}

void BitmapDrawer::writePixel(int c) {
	buff[(y*DISPLAY_WIDTH)+x] = toRGB(c);
	x++;
	if(x > endx) {
		x = startx;
		y++;
	}
	if(y > endy) {
		y = starty;
	}
//	printf("x:[%d <= %d < %d] y:[%d <= %d < %d]\n", startx, x, endx, starty, y, endy);
}

void BitmapDrawer::finishPixelWrite(void) {
  // Nothing should be needed
}

void BitmapDrawer::drawPoint(int x, int y, int c) {
  this->setupPixelWrite(x, y, x, y);
  this->writePixel(c);
  this->finishPixelWrite();
}

void BitmapDrawer::clear(unsigned int j) {
  printf("Clearing\n");
   this->setupPixelWrite(0,0,DISPLAY_WIDTH, DISPLAY_HEIGHT);

  for(int i=0;i<DISPLAY_HEIGHT;i++) {
    for(int m=0;m<DISPLAY_WIDTH;m++) {
       this->writePixel(j);
    }
  }
   this->finishPixelWrite();   
}

void BitmapDrawer::drawVline(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
	unsigned int i,j;
//	l=l+y;
	setupPixelWrite(x,y,1, l);
	j=l;
	for(i=1;i<=j;i++) {
		writePixel(c);
	}
	finishPixelWrite();
}

void BitmapDrawer::drawHline(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
	unsigned int i,j;
//	printf("> x: %d\ty: %d\tl: %d\tc: %d\n", x, y, l, c);
//	l=l+x;
//	printf("  x: %d\ty: %d\tl: %d\tc: %d\n", x, y, l, c);
	setupPixelWrite(x, y, l, 1);
	j=l;
	for(i=1;i<=j;i++) {
		writePixel(c);
	}
	finishPixelWrite();
}

void BitmapDrawer::drawRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) {
  drawHline(x  , y  , w, c);
  drawHline(x  , y+h, w, c);
  drawVline(x  , y  , h, c);
  drawVline(x+w, y  , h, c);
}

void BitmapDrawer::fillRect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) {
  unsigned int i;
  for(i=0;i<h;i++) {
//    this->drawHline(x  , y  , w, c);
    this->drawHline(x  , y+i, w, c);
  }
}

void BitmapDrawer::displaySprite(int xpos, int ypos, int* img, int w, int h, int scaleW, int scaleH) {
  printf("Drawing sprite: %d %d %d %d\n", xpos, ypos, xpos+(w*scaleW), ypos+(h*scaleH));
  this->setupPixelWrite(xpos, ypos, (w*scaleW), (h*scaleH));
  for(int q = 0; q < 480; q++) {
    //writePixel(GREEN);
  }
  for(int y = 0; y < h; y++) {
    for(int i = 0; i < scaleH; i++) {
      for(int x = 0; x < w; x++) {
        for(int j = 0; j < scaleW; j++) {
        
          this->writePixel(img[(y*w)+x] * 0xA800);
          //Serial.printf("%d %d %d %d %d\n", x, y, i, j, img[y*w+x]);
          printf("%d\n", img[y*w+x]);
        }
      }
      printf("\n");
    }
  }
  this->finishPixelWrite();
  printf("Done drawing sprite\n");
}

void BitmapDrawer::displayChar(int xpos, int ypos, uint8_t c, int scale, int fontcolor, int backgroundcolor) {
  this->setupPixelWrite(xpos, ypos, (CHAR_SIZE*scale), (CHAR_SIZE*scale));
  uint8_t* spt = &font[(0x21)*CHAR_SIZE];
  for(int y = 0; y < CHAR_SIZE; y++) {
    for(int yscale = 0; yscale < scale; yscale++) {
      int row = font[(((int) c)*CHAR_SIZE)+y];
      for(int x = 0; x < CHAR_SIZE; x++) {
        int px = ((row >> 7) & 1);
        row <<= 1;
        for(int xscale = 0; xscale < scale; xscale++) {
//        	printf("%c", (px) ? 'X' : '.');
          this->writePixel((px) ? fontcolor : backgroundcolor);
        }
      }
//      printf("\n");
    }
    spt++;
  }
  this->finishPixelWrite();
}

void BitmapDrawer::displayString(int xstart, int ystart, const char* str, int scale, int fontcolor, int backgroundcolor) {
  const char* c = str;
  int xpos = xstart;
  while(*c != '\0') {
    this->displayChar(xpos, ystart, *c, scale, fontcolor, backgroundcolor);
    xpos += (CHAR_SIZE*scale);
    c++;
  }
}

int num = 0;

void BitmapDrawer::screenshot(void) {
	char name[32];
	sprintf(name, "junk/%d.bmp", num++);
	outfile.open(name, std::ios::binary | std::ios::out);
	outfile.write((const char*) header, HEADER_LENGTH);
	for(int y = DISPLAY_HEIGHT-1; y >= 0; y--) {
		for(int x = 0; x < DISPLAY_WIDTH; x++) {
			outfile.write((const char*) &buff[(y*DISPLAY_WIDTH)+x], 3);
//			printf("[%02X,%02X,%02X] ", buff[(y*DISPLAY_WIDTH)+x].r, buff[(y*DISPLAY_WIDTH)+x].g, buff[(y*DISPLAY_WIDTH)+x].b);
		}
//		printf("\t\t\t\t\t\t\t\t\n");
	}
	outfile.close();
}
