#ifndef COLORS_H
#define COLORS_H

// Color values for easy use.
#define BLACK           0x0000 // 0
#define BLUE            0x0015 // 1
#define GREEN           0x0540 // 2
#define CYAN            0x0555 // 3
#define RED             0xA800 // 4
#define MAGENTA         0xA815 // 5
#define YELLOW          0xFBC1 //0xAAA0 // 6
#define LIGHT_GREY      0xAD55 // 7
#define DARK_GREY       0x52AA // 8
#define BRIGHT_BLUE     0x001F // 9
#define BRIGHT_GREEN    0x07E0 // A
#define BRIGHT_CYAN     0x07FF // B
#define BRIGHT_RED      0xF800 // C
#define BRIGHT_MAGENTA  0xF81F // D
#define BRIGHT_YELLOW   0xFFE0 // E
#define WHITE           0xFFFF // F

// Origional EGA colors
#define ORIGIONAL_BRIGHT_BLUE     0x52BF
#define ORIGIONAL_BRIGHT_GREEN    0x57EA
#define ORIGIONAL_BRIGHT_CYAN     0x57FF
#define ORIGIONAL_BRIGHT_RED      0xFAAA
#define ORIGIONAL_BRIGHT_MAGENTA  0xFABF
#define ORIGIONAL_BRIGHT_YELLOW   0xFFEA
#define WHITE           0xFFFF // F

#define PURE_RED
#define PURE_GREEN
#define PURE_BLUE

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

extern const int egaColors[16];

#endif
