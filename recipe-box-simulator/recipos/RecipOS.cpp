#include "RecipOS.h"

#include <string.h>

#include "../arduinostandin.h"

#include "drivers/display/bitmapdrawer.h"

#include "modules/Buttons.h"
#include "modules/Display.h"
#include "modules/Printer.h"
#include "modules/Speaker.h"
#include "modules/Storage.h"

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

int RecipOS::addTab(TabApp* tab, void* box) {
	for(int i = 0; i < MAX_TABS; i++) {
		if(tabs[i] == NULL) {
			tabs[i] = tab;
			if(box != NULL) {
				tab->box = box;
			}
			Display* display = createAppDisplay();
			tabDisplays[i] = display;
			return i;
		}
	}
	return -1;
}

Display* RecipOS::createAppDisplay(void) {
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
				tabDisplays[appid] = createAppDisplay();
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
				serviceInterval = 1000; // Check buttons 10x per second
			}
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
