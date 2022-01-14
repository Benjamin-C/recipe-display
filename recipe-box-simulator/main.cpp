#include "main.h"
#include <stdio.h>

#include "arduinostandin.h"
#include "lcdtest.h"
#include "recipos/drivers/display/bitmapdrawer.h"
#include "recipos/modules/Display.h"
#include "recipos/RecipOS.h"
#include "recipos/Application.h"

#include "recipos/colors.h"

#include <string.h>

int main(void) {
	printf("Hello World %ld\n", sizeof(char));

	setup();

	RecipOS ros = RecipOS();

	class MyApp : public Application {
	public:
		~MyApp() { };
		void startup() {
			strcpy((char*) name, "Test Application");
//			abriv[4] = '\0';
//			strcpy((char*) abriv, "qqqq");
			// Make sure it looks good on black
//			color = EGA_BRIGHT_RED;
		}
		void paint(Display* d) {
			printf("color: %d %s\n", color, abriv);
			d->fill(egaColors[color]);
			d->displayString(0,0,name,2,WHITE,egaColors[color]);
			return;
		}
		void load(void) {
			printf("App loaded! %d\n", color);
		}
		void onButtonPress(uint16_t pressed, Buttons* buttons) {
			printf("Someone pushed my button!\n");
		}
	};

	int testCount = 6;

	ros.boot();

	ros.mainDisplay->screenshot();

	for(int i = 0; i < testCount; i++) {
		MyApp* app = new MyApp();
		sprintf(app->abriv, "Tab%01d ", i);
//		app->abriv[2] = '\0';
		app->color = i+3;
		ros.addApplication(app);
	}

	for(int i = 0; i < testCount; i++) {
		ros.switchApp(i);
		ros.checkButtonPress();
		delay(3000);
		ros.mainDisplay->screenshot();
		printf("Should have saved screenshot\n");
	}

	printf("Done!");


//	runDemo();

//	lcd.screenshot();

//	delay(5000);

//	lcd.fill(0xFFFF);
//	lcd.displayString(50, 257, "Thank you", 4, lcd.RGB(255, 0, 0), BLACK);
//	lcd.displayString(50, 82, "for watching", 4, lcd.RGB(255, 0, 0), BLACK);
//
//	lcd.screenshot();

//	TouchLCD test;
//	test.displayString(0, 0, "This is text", 2, 0xA800, 0);
//	test.screenshot();
}
