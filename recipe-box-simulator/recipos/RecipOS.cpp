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
			return i;
		}
	}
	return -1;
}

bool RecipOS::switchApp(int appid) {
	// send old app to backgorund, can't do yet
	if(appid >= 0 && appid < MAX_APPLICATIONS) {
		apps[appid]->paint(mainDisplay);
		currentApp = appid;
		printf("Starting app [%s] %s\n", apps[appid]->abriv, apps[appid]->name);
		return true;
	}
	return false;
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
		for(int i = 0; i < MAX_APPLICATIONS; i++) {
			if(apps[i] != NULL) {
				currentApp = i;
				apps[i]->startup();
				booted = true;
			}
			switchApp(currentApp);
			return true;
		}
		printf("Add an app before booting");
		return false;
	} else {
		printf("Can't boot, already booted!");
		return false;
	}
}
