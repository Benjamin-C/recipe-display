#include "RecipOS.h"

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
	for(int i = 0; i < MAX_APPLICATIONS; i++) {
		apps[i] = NULL;
	}
	for(int i = 0; i < MAX_WIDGETS; i++) {
		widgets[i] = NULL;
	}
}

int RecipOS::addApplication(Application* app) {
	for(int i = 0; i < MAX_APPLICATIONS; i++) {
		if(apps[i] == NULL) {
			apps[i] = app;
			Display* display = createAppDisplay();
			appDisplays[i] = display;
			return i;
		}
	}
	return -1;
}

Display* RecipOS::createAppDisplay(void) {
	return new Display(displayBackend, 0, WIDGET_HEIGHT, displayBackend->width, displayBackend->height - TAB_HEIGHT);
}

bool RecipOS::switchApp(int appid) {
	// send old app to backgorund, can't do yet
	if(appid >= 0 && appid < MAX_APPLICATIONS) {
		Application* app = apps[appid];
		if(app != NULL) {
			if(currentApp >= 0 && currentApp < MAX_APPLICATIONS) {
				appDisplays[currentApp]->setEnabled(false);
			}

			Display* display = appDisplays[appid];

			if(display != NULL) {
				appDisplays[appid] = createAppDisplay();
				display = appDisplays[appid];
			}

			display->setEnabled(true);

			if(!app->running) {
				app->startup();
				app->running = true;
			}

			app->load();
			app->paint(appDisplays[appid]);
			currentApp = appid;
			drawTabs();
			printf("Starting app [%s] %s\n", apps[appid]->abriv, apps[appid]->name);
			return true;
		} else {
			printf("Can't switch, something was null\n");
			return false;
		}
	}
	return false;
}

bool RecipOS::drawTabs(void) {
//	printf("hh");
	int y = displayBackend->height - TAB_HEIGHT;
	int x = 0;
	int dx = displayBackend->width / NUM_TABS;
	for(int i = 0; i < MAX_APPLICATIONS; i++) {
		Application* app = apps[i];
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
			mainDisplay->displayString(x, y, (const char*) abriv, 2, (i == currentApp) ? BLACK : color, (i == currentApp) ? color : BLACK);
			x += dx;
		}
	}
	drawWidgets();
	return true;printf("Was %d, now is %d\n", currentApp, currentApp-1);
}

bool RecipOS::tabLeft(void) {
	if(currentApp > 0) {
		printf(" < Was %d, now is %d\n", currentApp, currentApp-1);
		switchApp(currentApp-1);
		return true;
	}
	return false;
}

bool RecipOS::tabRight(void) {
	if(apps[currentApp+1] != NULL) {
		printf(" > Was %d, now is %d\n", currentApp, currentApp+1);
		switchApp(currentApp+1);
		return true;
	}
	return false;
}

bool RecipOS::drawWidgets(void) {
	mainDisplay->displayString(0, 0, "WIDGETS GO HERE", 4, BRIGHT_GREEN, BLACK);
	return true;
}

int RecipOS::addWidget(Application* widget) {
	for(int i = 0; i < MAX_WIDGETS; i++) {
		if(widgets[i] == NULL) {
			widgets[i] = widget;
			return i;
		}
	}
	return -1;
}

bool RecipOS::boot(void) {
	if(!booted) {
		bool hasFirst = false;
		for(int i = 0; i < MAX_APPLICATIONS; i++) {
			if(apps[i] != NULL) {
				apps[i]->startup();
				if(!hasFirst) {
					hasFirst = true;
					currentApp = i;

				}
			}
		}
		displayBackend->displayString(24, 128, "RecipOS", 8, BRIGHT_RED, BLACK);
		if(currentApp >= 0 && currentApp <= MAX_APPLICATIONS) {
			switchApp(currentApp);
		}
		booted = true;
		return true;
	} else {
		printf("Can't boot, already booted!");
		return false;
	}
}

bool RecipOS::checkButtonPress(void) {
	printf("Press button?\n");

	uint16_t pressed = buttons->checkButtons();
	if(pressed > 0) {
		int i = currentApp;
//		for(int i = 0; i < MAX_APPLICATIONS; i++) {
		if(apps[i] != NULL) {
			apps[i]->onButtonPress(pressed, buttons);
		}
//		}
		return true;
	} else {
		return false;
	}
}
