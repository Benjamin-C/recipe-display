#include "main.h"
#include <stdio.h>

#include "arduinostandin.h"
#include "recipos/drivers/display/bitmapdrawer.h"
#include "recipos/modules/Display.h"
#include "recipos/RecipOS.h"
#include "recipos/Application.h"

#include "recipos/colors.h"

#include <string.h>
#include <string>

#include "software/RecipeUtils.h"
#include "software/RecipeSelector.h"

#define DEBUG(val) printf("%s\n", val)

int main(void) {
	String noihatethis = "No, I hate this!";

	printf("Hello World %ld\n", sizeof(char));

//	setup();
/*
	DEBUG("Making RO");
	RecipeUtils::Recipe* ro = RecipeUtils::getExampleRecipe();
	DEBUG("Printing RO");
	RecipeUtils::printRecipe(ro);
*/
	DEBUG("Making OS");
	RecipOS ros = RecipOS();
	DEBUG("Making MyApp");

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
		void onMessage(int mid, std::string dest, void* mbox) {
			if(mid != lastMID) {
				lastMID = mid;
				if(dest == "testmsg") {
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
//				RecipeUtils::parseStrin(os->storage->readFile("/dev/example"));
				printf("\n%s\n", os->storage->readFile("b.txt").c_str());
			} else if((pressed & BUTTON_UP_MASK) > 0) {
				makeBSOD("You pressed up. How dare you! ");
			}
			os->sendMessage(os->getNextMID(), "testmsg", (void*) &pressed);
			printf("Someone pushed my button!\n");
			os->displayBackend->screenshot();
		}

		void runService(void) { }
		void paintWidget(Display* d) { }
	};

	DEBUG("Making Timer");

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
		void onMessage(int mid, std::string dest, void* mbox) {
			if(mid != lastMID) {
				lastMID = mid;
				if(dest == "timer") {
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

	DEBUG("Adding MyApps");

	for(int i = 0; i < testCount; i++) {
		MyApp* app = new MyApp();
		void* c = malloc(sizeof(int));
		if(c != NULL) {
			*((int*) c) = i;
			ros.addTab(app, c);
			printf("Added app I think\n");
		}
	}

	DEBUG("Making Screenshotter");

	class StartupScreenshot : public Application {
	public:
		~StartupScreenshot() { };
		void startup(RecipOS* os) {
			this->os = os;
			serviceInterval = 100;
		}
		int lastMID = -1;
		void onMessage(int mid, std::string dest, void* mbox) { }
		void paintTab(Display* d) { }
		void runTab(void) { }
		void onButtonPress(uint16_t pressed, Buttons* buttons) { }
		void runService(void) {
			os->displayBackend->screenshot();
			serviceInterval = -1;

			Serial.println("Say Cheese!");

			Serial.println("File listing:");
			  FileList fl = os->storage->getDirectoryList("/");

			  for(int i = 0; i < fl.count; i++) {
			    Serial.print(fl.list[i].c_str());
			    Serial.print("\t\t");
			//    File entry = SD.open(fl.list[i].c_str());
			    int qtcnt = 0;
			    std::string fle = os->storage->readFile(fl.list[i]);
			    const char* c = fle.c_str();
			    while(qtcnt < 6) {
			      if(*c == '"') {
			        qtcnt++;
			      } else {
			        if(qtcnt == 5) {
			          Serial.printf("%c", *c);
			        }
			      }
			      c++;
			    }
			//    entry.close();
			    Serial.println();
			  }
		}
		void paintWidget(Display* d) { }
	};

	DEBUG("Starting Screenshotter");

	StartupScreenshot* sst = new StartupScreenshot();
	ros.addService(sst, NULL);

	DEBUG("Making Timer");

	Timer* timer = new Timer();

	DEBUG("Adding timer");

	ros.addTab(timer, NULL);
	ros.addService(timer, NULL);

	DEBUG("Adding RecipeSelector");
/*
	RecipeSelector* rs = new RecipeSelector();
	int tid = ros.addTab(rs, NULL);
	ros.switchTab(tid);
*/
	DEBUG("Booting");

	printf("Boot\n");
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
