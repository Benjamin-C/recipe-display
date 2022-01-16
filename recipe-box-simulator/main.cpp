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
		void startup(RecipOS* os) {
			this->os = os;
			strcpy(name, "Test Application");
			int i = *((int*) box);
			printf("Startup: %d\n", i);
			sprintf(abriv, "Tab%01d", i);
			color = i+3;
		}
		void paintTab(Display* d) {
			printf("color: %d %s\n", color, abriv);
			d->fill(egaColors[color]);
			d->displayString(0,0,name,2,WHITE,egaColors[color]);
			return;
		}
		void runTab(void) {
			printf("App loaded! %d\n", color);
		}
		void onButtonPress(uint16_t pressed, Buttons* buttons) {
			if((pressed & BUTTON_RIGHT_MASK) > 0) {
				os->tabRight();
				printf("Right\n");
			} else if((pressed & BUTTON_LEFT_MASK) > 0) {
				os->tabLeft();
				printf("Left\n");
			}
			printf("Someone pushed my button!\n");
			os->displayBackend->screenshot();
		}

		void runService(void) { }
		void paintWidget(Display* d) { }
	};

	int testCount = 6;

	for(int i = 0; i < testCount; i++) {
		MyApp* app = new MyApp();
		void* c = malloc(sizeof(int));
		if(c != NULL) {
			*((int*) c) = i;
			ros.addTab(app, c);
		}
	}

	ros.boot(); // Program will not pass here

//	ros.switchTab(0);
//
//	ros.mainDisplay->screenshot();
//
//	while(true) {
//		if(ros.checkButtonPress()) {
//			ros.mainDisplay->screenshot();
//		}
//	}
//
//	for(int i = 0; i < testCount; i++) {
//		ros.switchTab(i);
//		ros.checkButtonPress();
//		delay(3000);
//		ros.mainDisplay->screenshot();
//		printf("Should have saved screenshot\n");
//	}
//
//	printf("Done!");


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
