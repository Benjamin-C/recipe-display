#include "main.h"
#include <stdio.h>

#include "arduinostandin.h"
#include "lcdtest.h"
#include "recipos/drivers/display/bitmapdrawer.h"
#include "recipos/modules/Display.h"
#include "recipos/RecipOS.h"
#include "recipos/Application.h"

int main(void) {
	printf("Hello World %ld\n", sizeof(char));

	setup();

	RecipOS ros = RecipOS();

	class MyApp : public Application {
	public:
		~MyApp() { };
		void startup() {
			name = "Test Application";
			abriv = "TSTAPP";
		}
		void paint(Display* d) {
			d->fill(0xFF00);
			return;
		};

	};

	MyApp* myapp = new MyApp();

	ros.addApplication(myapp);

	ros.boot();

	ros.mainDisplay->screenshot();
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
