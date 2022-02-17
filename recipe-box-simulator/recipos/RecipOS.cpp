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

#include "drivers/display/bitmapdrawer.h"

#include "modules/Buttons.h"
#include "modules/Display.h"
#include "modules/Printer.h"
#include "modules/Speaker.h"
#include "modules/Storage.h"

RecipOSErrorWrapper* globalError;

RecipOS::RecipOS() {
	displayBackend = new BitmapDrawer();
	mainDisplay = new Display(displayBackend, 0, 0, 480, 320);
	mainDisplay->setEnabled(true);
	buttons = new Buttons();
	printer = new Printer();
	speaker = new Speaker();
	storage = new Storage();
	for(int i = 0; i < MAX_TABS; i++) {
		tabs[i] = NULL;
	}
	for(int i = 0; i < MAX_WIDGETS; i++) {
		widgets[i] = NULL;
	}
	for(int i = 0; i < MAX_SERVICES; i++) {
		services[i] = NULL;
	}



}

int RecipOS::getNextMID(void) {
	return nextMID++;
}

void RecipOS::sendMessage(int mid, const char* dest, void* msgbox) {
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
			app->paintTab(tabDisplays[appid]);
			currentTab = appid;
			drawTabList();
			printf("Starting app [%s] %s\n", tabs[appid]->abriv, tabs[appid]->name);
			return true;
		} else {
			printf("Can't switch, something was null\n");
			return false;
		}
	}
	return false;
}

bool RecipOS::repaintCurrentTab(void) {
	TabApp* app = tabs[currentTab];
	if(app != NULL) {
		app->paintTab(tabDisplays[currentTab]);
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
	mainDisplay->displayString(0, 0, "WIDGETS GO HERE", 4, BRIGHT_GREEN, BLACK);
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
			if(ser->nextServiceTime < millis()) {
				ser->runService();
				while(ser->nextServiceTime < millis()) {
					ser->nextServiceTime += ser->serviceInterval;
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

		if(globalError->hasError) {
			bsod(globalError->errMsg);
		}

		bool hasFirst = false;
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
			void onMessage(int mid, const char* dest, void* mbox) { }
			void paintTab(Display* d) { }
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
			void onMessage(int mid, const char* dest, void* mbox) { }
			void paintTab(Display* d) { }
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
	if(pressed > 0) {
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

void RecipOSErrorWrapper::setOS(RecipOS* os) {
	myROS = os;
}

void RecipOSErrorWrapper::bsod(std::string msg) {
	if(!hasError) {
		errMsg = msg;
		hasError = true;
		if(myROS != NULL) {
			myROS->bsod(msg);
		}
	} else {
		printf("There is already a BSOD queued. Only the first error is shown on the screen.\n");
		printf("This error msg: %s\n", msg.c_str());
	}
}

void makeBSOD(std::string msg) {
	if(globalError == NULL) {
		globalError = new RecipOSErrorWrapper;
	}
	globalError->bsod(msg);
}
