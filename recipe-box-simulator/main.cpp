#include "main.h"
#include <stdio.h>

#include "arduinostandin.h"
#include "lcdtest.h"
#include "recipos/drivers/display/bitmapdrawer.h"
#include "recipos/modules/display.h"

int main(void) {
	printf("Hello World %ld\n", sizeof(char));

	setup();
	runDemo();

//	lcd.screenshot();

//	delay(5000);

	lcd.clear(0xFFFF);
	lcd.displayString(50, 257, "Thank you", 4, lcd.RGB(255, 0, 0), BLACK);
	lcd.displayString(50, 82, "for watching", 4, lcd.RGB(255, 0, 0), BLACK);

	lcd.screenshot();

//	TouchLCD test;
//	test.displayString(0, 0, "This is text", 2, 0xA800, 0);
//	test.screenshot();
}
