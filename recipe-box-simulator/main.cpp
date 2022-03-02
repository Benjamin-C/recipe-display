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

#include "software/RecipeUtils.h"

int main(void) {
	printf("Hello World %ld\n", sizeof(char));

//	setup();

	//Recipe* ro = getExampleRO();
	//printRecipeObject(ro);

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
		int lastMID = -1;
		void onMessage(int mid, const char* dest, void* mbox) {
			if(mid != lastMID) {
				lastMID = mid;
				if(strcmp((const char*) dest, "testmsg")==0) {
					printf("Button press message! %d\n", *((uint16_t*) mbox));
				}
			}
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
			} else if((pressed & BUTTON_DOWN_MASK) > 0) {
				char str[1024];
				os->storage->readFile("/dev/example", str, 0, 1024);
				printf("%s\n", str);
			}
			os->sendMessage(os->getNextMID(), "testmsg", (void*) &pressed);
			printf("Someone pushed my button!\n");
			os->displayBackend->screenshot();
		}

		void runService(void) { }
		void paintWidget(Display* d) { }
	};

	class Timer : public Application {
	public:
		int runtime = -1;
		bool timerRunning = false;
		~Timer() { };
		void startup(RecipOS* os) {
			this->os = os;
			strcpy(name, "Timer");
			strcpy(abriv, "Timr");
			this->color = EGA_BRIGHT_GREEN;
			serviceInterval = 1000;
		}
		int lastMID = -1;
		void onMessage(int mid, const char* dest, void* mbox) {
			if(mid != lastMID) {
				lastMID = mid;
				if(strcmp((const char*) dest, "timer")==0) {
					void** args = (void**) mbox;
					const char* cmd = (const char*) args[0];
					printf("Timer: %s\n", cmd);
					runtime = *((int*) args[1]);
					printf("Time: %d\n", runtime);
					timerRunning = true;
					nextServiceTime = millis() + 1000;
					os->repaintCurrentTab();
					os->displayBackend->screenshot();
				}
			}
		}
		void paintTab(Display* d) {
			d->fill(egaColors[color]);
			d->displayString(0,0,name,2,WHITE,egaColors[color]);
			if(runtime > 0) {
				char time[8];
				sprintf(time, "%d", runtime);
				d->displayString(0, 16, time, 2, WHITE, egaColors[color]);
			} else {
				d->displayString(0, 16, "Stopped", 2, WHITE, egaColors[color]);
			}
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
			} else if((pressed & BUTTON_UP_MASK) > 0) {
				void* msgbox[2];
				const char* cmd = "start";
				msgbox[0] = (void*) cmd;
				int stime = 3;
				msgbox[1] = (void*) &stime;
				os->sendMessage(os->getNextMID(), "timer", msgbox);
			}

			printf("Someone pushed my button!\n");
			os->displayBackend->screenshot();
		}

		void runService(void) {
			if(timerRunning) {
				runtime--;
				if(runtime <= 0) {
					printf("Timer done!\n");
					timerRunning = false;
				}
				os->repaintCurrentTab();
				os->displayBackend->screenshot();
			}
		}
		void paintWidget(Display* d) { }
	};

	int testCount = 3;

	for(int i = 0; i < testCount; i++) {
		MyApp* app = new MyApp();
		void* c = malloc(sizeof(int));
		if(c != NULL) {
			*((int*) c) = i;
			ros.addTab(app, c);
		}
	}

	Timer* timer = new Timer();

	int tid = ros.addTab(timer, NULL);
	ros.addService(timer, NULL);
	ros.switchTab(tid);

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
