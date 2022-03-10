/* RecipOS.cpp
 *
 * Operating system for the Recipe Box device
 *
 * This class handles most of the running of the box, including
 *   drawing windows, managing apps, running services, user input,
 *   outputs, and more.
 *
 * Author: Benjamin Crall
 */

#include "RecipOS.h"

#include <string.h>

#include "../arduinostandin.h"

#include "drivers/display/touchlcd.h"
#include "drivers/storage/ESP32SDCard.h"
#include "drivers/buttons/ButtonReader.h"

#include "modules/Buttons.h"
#include "modules/Display.h"
#include "modules/Printer.h"
#include "modules/Speaker.h"
#include "modules/Storage.h"

RecipOSErrorWrapper* globalError;

RecipOS::RecipOS() {
	displayBackend = new TouchLCD();
	mainDisplay = new Display(displayBackend, 0, 0, 480, 320);
	mainDisplay->setEnabled(true);
	buttons = new Buttons();
	buttons->bb = new ButtonReader();
	printer = new Printer();
	speaker = new Speaker();
	storage = new Storage();
	storage->sb = new ESP32SDCard();
	for(int i = 0; i < MAX_TABS; i++) {
		tabs[i] = NULL;
	}
	for(int i = 0; i < MAX_WIDGETS; i++) {
		widgets[i] = NULL;
	}
	for(int i = 0; i < MAX_SERVICES; i++) {
		services[i] = NULL;
	}

	lastButtons = -1;

}

int RecipOS::getNextMID(void) {
	return nextMID++;
}

void RecipOS::sendMessage(int mid, std::string dest, void* msgbox) {
	for(int i = 0; i < MAX_TABS; i++) {
		if(tabs[i] != NULL) {
			tabs[i]->onMessage(mid, dest, msgbox);
		}
	}
	for(int i = 0; i < MAX_WIDGETS; i++) {
		if(widgets[i] != NULL) {
			widgets[i]->onMessage(mid, dest, msgbox);
		}
	}
	for(int i = 0; i < MAX_SERVICES; i++) {
		if(services[i] != NULL) {
			services[i]->onMessage(mid, dest, msgbox);
		}
	}
}

int RecipOS::addTab(TabApp* tab, void* box) {
	for(int i = 0; i < MAX_TABS; i++) {
		if(tabs[i] == NULL) {
			tabs[i] = tab;
			if(box != NULL) {
				tab->box = box;
			}
			Display* display = createTabDisplay();
			tabDisplays[i] = display;
			return i;
		}
	}
	return -1;
}

Display* RecipOS::createTabDisplay(void) {
	return new Display(displayBackend, 0, WIDGET_HEIGHT, displayBackend->width, displayBackend->height - TAB_HEIGHT);
}

bool RecipOS::switchTab(int appid) {
	// send old app to backgorund, can't do yet
	if(appid >= 0 && appid < MAX_TABS) {
		TabApp* app = tabs[appid];
		if(app != NULL) {
			if(currentTab >= 0 && currentTab < MAX_TABS) {
				tabDisplays[currentTab]->setEnabled(false);
			}

			Display* display = tabDisplays[appid];

			if(display != NULL) {
				tabDisplays[appid] = createTabDisplay();
				display = tabDisplays[appid];
			}

			display->setEnabled(true);

			if(!app->running) {
				app->startup(this);
				app->running = true;
			}

			app->runTab();
			app->paintTab(tabDisplays[appid], false);
			currentTab = appid;
			drawTabList();
			printf("Starting app [%s] %s\n", tabs[appid]->abriv, tabs[appid]->name);
			return true;
		} else {
			printf("Can't switch, something was null\n");
			return false;
		}
	} else {
		printf("Invalid dest");
	}
	return false;
}

bool RecipOS::repaintCurrentTab(void) {
	TabApp* app = tabs[currentTab];
	if(app != NULL) {
		app->paintTab(tabDisplays[currentTab], true);
		return true;
	} else {
		return false;
	}
}

bool RecipOS::drawTabList(void) {
//	printf("hh");
	int y = displayBackend->height - TAB_HEIGHT;
	int x = 0;
	int dx = displayBackend->width / NUM_TABS;
	for(int i = 0; i < MAX_TABS; i++) {
		TabApp* app = tabs[i];
		if(app != NULL) {
			int egacolor = app->color;
			int color = egaColors[egacolor];
			printf("Printing tab %s %d\n", app->abriv, color);
			char* abriv = app->abriv;
			bool end = false;
			for(int i = 0; i < MAX_ABRIV_LENGTH; i++) {
				if(abriv[i] == '\0' || end) {
					abriv[i] = ' ';
					end = true;
				}
			}
			abriv[MAX_ABRIV_LENGTH] = '\0';
			mainDisplay->displayString(x, y, (const char*) abriv, 2, (i == currentTab) ? BLACK : color, (i == currentTab) ? color : BLACK);
			x += dx;
		}
	}
	drawWidgets();
	return true;printf("Was %d, now is %d\n", currentTab, currentTab-1);
}

bool RecipOS::tabLeft(void) {
	if(currentTab > 0) {
		printf(" < Was %d, now is %d\n", currentTab, currentTab-1);
		switchTab(currentTab-1);
		return true;
	}
	return false;
}

bool RecipOS::tabRight(void) {
	if(tabs[currentTab+1] != NULL) {
		printf(" > Was %d, now is %d\n", currentTab, currentTab+1);
		switchTab(currentTab+1);
		return true;
	}
	return false;
}

bool RecipOS::drawWidgets(void) {
//	WidgetApp
//	if(!service->running) {
//					service->startup(this);
//				}
	mainDisplay->displayString(0, 0, " Tasty  Snacks ", 4, DARK_GREY, BLACK);
	return true;
}

int RecipOS::addWidget(WidgetApp* widget, void* box) {
	for(int i = 0; i < MAX_WIDGETS; i++) {
		if(widgets[i] == NULL) {
			widgets[i] = widget;
			if(box != NULL) {
				widget->box = box;
			}
			return i;
		}
	}
	return -1;
}

int RecipOS::addService(ServiceApp* service, void* box) {
	for(int i = 0; i < MAX_SERVICES; i++) {
		if(services[i] == NULL) {
			services[i] = service;
			if(!service->running) {
				service->startup(this);
				service->running = true;
			}
			service->nextServiceTime = millis();
			if(box != NULL) {
				service->box = box;
			}
			return i;
		}
	}
	return -1;
}

bool RecipOS::runServices(void) {
	for(int i = 0; i < MAX_SERVICES; i++) {
		if(services[i] != NULL) {
			ServiceApp* ser = services[i];
			if(!ser->running) {
				ser->startup(this);
				ser->running = true;
			}
			if(ser->serviceInterval >= 0 && ser->nextServiceTime < millis()) {
				ser->runService();
				if(ser->serviceInterval > 0) {
					while(ser->nextServiceTime < millis()) {
						ser->nextServiceTime += ser->serviceInterval;
					}
				}
			}
		}
		delay(1); // Use a bit less cpu power, may or may not be needed
	}
	return true;
}

bool RecipOS::boot(void) {
	if(!booted) {
		if(globalError == NULL) {
			globalError = new RecipOSErrorWrapper();
		}
		globalError->setOS(this);

		if(globalError->hasBSOD) {
			bsod(globalError->errMsg);
		}

		if(globalError->hasErr) {
			error(globalError->errMsg);
		}

		bool hasFirst = currentTab >= 0;
		for(int i = 0; i < MAX_TABS; i++) {
			if(tabs[i] != NULL) {
				tabs[i]->startup(this);
				tabs[i]->running = true;
				if(!hasFirst) {
					hasFirst = true;
					currentTab = i;

				}
			}
		}
		displayBackend->clear(BLACK);
		displayBackend->displayString(24, 128, "RecipOS", 8, BRIGHT_RED, BLACK);
		if(currentTab >= 0 && currentTab <= MAX_TABS) {
			switchTab(currentTab);
		}
		booted = true;

		class ButtonService : public ServiceApp {
		public:
			~ButtonService() { };
			void startup(RecipOS* os) {
				this->os = os;
				serviceInterval = 100; // Check buttons 10x per second
			}
			void onMessage(int mid, std::string dest, void* mbox) { }
			void paintTab(Display* d, bool repaint) { }
			void runTab(void) { }
			void onButtonPress(uint16_t pressed, Buttons* buttons) { }
			void paintWidget(Display* d) { }
			void runService(void) { os->checkButtonPress(); }
		};

		ButtonService* bts = new ButtonService();
		addService(bts, NULL);

		class WatchdogService : public ServiceApp {
		public:
			~WatchdogService() { };
			void startup(RecipOS* os) {
				this->os = os;
				serviceInterval = 1000; // Woof every second
			}
			void onMessage(int mid, std::string dest, void* mbox) { }
			void paintTab(Display* d, bool repaint) { }
			void runTab(void) { }
			void onButtonPress(uint16_t pressed, Buttons* buttons) { }
			void paintWidget(Display* d) { }
			void runService(void) { printf("Woof\n"); }
		};

		WatchdogService* wds = new WatchdogService();
		addService(wds, NULL);

		while(booted) {
			runServices();
		}

		return true;
	} else {
		printf("Can't boot, already booted!");
		return false;
	}
}

bool RecipOS::checkButtonPress(void) {
//	printf("Press button?\n");

	uint16_t pressed = buttons->checkButtons();
	if(pressed != lastButtons) {
		lastButtons = pressed;
		if(pressed > 0) {
			printf("Pressed: %d\n", pressed);
			int i = currentTab;
	//		for(int i = 0; i < MAX_APPLICATIONS; i++) {
			if(tabs[i] != NULL) {
				tabs[i]->onButtonPress(pressed, buttons);
			}
	//		}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// BSOD stuffs
void RecipOS::bsod(std::string msg) {
	buttons->halt(); // Should probably shut other sutff down here too
	displayBackend->clear(BLUE);
	displayBackend->displayString(0, 0, "   ---   Fatal  Error   ---   ", 2, WHITE, BLUE);
	printf("\n\n\n-------- RecipOS has crashed! --------\n");

	// Some nice multiline drawing code that maybe should be implemented into the display stuff?
	const char* c = msg.c_str();
	bool hasMoreMsg = true;
	int ystart = 16;
	const int SCALE = 2;
	while(hasMoreMsg) {
		int xpos = 0;
		int rowcount = 0;
		while(*c != '\0' && rowcount < 60/SCALE) {
			displayBackend->displayChar(xpos, ystart, *c, SCALE, WHITE, BLUE);
			printf("%c", *c);
			xpos += 8*SCALE;
			c++;
			rowcount++;
		}
		if(*c != '\0') {
			ystart += 8*SCALE;
		} else {
			hasMoreMsg = false;
		}
		printf("\n");
	}
	printf("--------------------------------------\n");
	displayBackend->screenshot();
	while(true) { // OS is crashed, so don't do anything else.
		delay(2147483647); // Basically forever anyway
	} // but then loop so it really is forever
}

void RecipOS::error(std::string msg) {
	const int FONTCOLOR = BLACK;
	const int BACKGROUND = displayBackend->RGB(255, 255, 0); // Yellow

	int leftx = 3*16;
	displayBackend->displayString(leftx, 3*16, "\xC9\xCD\xCD\xCD\xCD Error \xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB", 2, FONTCOLOR, BACKGROUND);
	printf("\n\n\n-------- Something has encountered an error! --------\n");

	// Some nice multiline drawing code that maybe should be implemented into the display stuff?

	const char* c = msg.c_str();
	bool hasMoreMsg = true;
	const int SCALE = 2;
	int rows = 0;
	int ystart = 4*16;
	while(rows < 12) {
		int xpos = leftx;
		int charCount = 0;
		displayBackend->displayChar(xpos, ystart, '\xBA', SCALE, FONTCOLOR, BACKGROUND);
		xpos += 8*SCALE;
		displayBackend->displayChar(xpos, ystart, ' ', SCALE, FONTCOLOR, BACKGROUND);
		xpos += 8*SCALE;
		const int LINEWIDTH = 20;
		int ll = 0;
		int tll = 0;
		const char* tc = c;
		while(tll <= LINEWIDTH) {
			while(*tc != ' ' && *tc != '\0' && *tc != '\n') { tc++; tll++; }
			if(tll <= LINEWIDTH) {
				ll = tll;
				if(*tc != ' ') {
					break;
				} else {
					tc++;
					tll++;
				}
			}
		}
		while(charCount < LINEWIDTH) {
			if(*c != '\0' && *c != '\n' && ll > 0) {
				displayBackend->displayChar(xpos, ystart, *c, SCALE, FONTCOLOR, BACKGROUND);
				printf("%c", *c);
				c++;
				ll--;
				if(ll == 0 && *c != '\0') {
					c++;
				}
			} else {
				if(*c == '\0') {
					hasMoreMsg = false;
				}
				displayBackend->displayChar(xpos, ystart, ' ', SCALE, FONTCOLOR, BACKGROUND);
			}
			xpos += 8*SCALE;
			charCount++;
		}
		if(*c == '\n') {
			c++;
		}
		displayBackend->displayChar(xpos, ystart, ' ', SCALE, FONTCOLOR, BACKGROUND);
		xpos += 8*SCALE;
		displayBackend->displayChar(xpos, ystart, '\xBA', SCALE, FONTCOLOR, BACKGROUND);
		ystart += 8*SCALE;
		if(hasMoreMsg) {
			printf("\n");
		}
		rows++;
	}
	displayBackend->displayString(leftx, 16*16, "\xC8", 2, FONTCOLOR, BACKGROUND);
	for(int i = 1; i < 23; i++) {
		displayBackend->displayString(leftx+(16*i), 16*16, "\xCD", 2, FONTCOLOR, BACKGROUND);
	}
	displayBackend->displayString(leftx+(16*23), 16*16, "\xBC", 2, FONTCOLOR, BACKGROUND);
	printf("\n--------------------------------------\n");
	displayBackend->screenshot();
}

void RecipOSErrorWrapper::setOS(RecipOS* os) {
	myROS = os;
}

void RecipOSErrorWrapper::bsod(std::string msg) {
	if(!hasBSOD) {
		errMsg = msg;
		hasBSOD = true;
		if(myROS != NULL) {
			myROS->bsod(msg);
		}
	} else {
		printf("There is already a BSOD queued. Only the first error is shown on the screen.\n");
		printf("This error msg: %s\n", msg.c_str());
	}
}

void RecipOSErrorWrapper::error(std::string msg) {
	if(!hasBSOD) {
		if(!hasErr) {
			errMsg = msg;
			hasErr = true;
			if(myROS != NULL) {
				myROS->error(msg);
			}
		} else {
			printf("There is already another error queued. Only the first error is shown on the screen.\n");
			printf("This error msg: %s\n", msg.c_str());
		}
	} else {
		printf("There is already a BSOD queued. Only the first error is shown on the screen.\n");
		printf("This error msg: %s\n", msg.c_str());
	}
}

void makeError(std::string msg) {
	if(globalError == NULL) {
		globalError = new RecipOSErrorWrapper;
	}
	globalError->error(msg);
}

void makeBSOD(std::string msg) {
	if(globalError == NULL) {
		globalError = new RecipOSErrorWrapper;
	}
	globalError->bsod(msg);
}
