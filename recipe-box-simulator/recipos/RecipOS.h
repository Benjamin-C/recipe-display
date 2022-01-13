#ifndef RECIPOS_H
#define RECIPOS_H

#include "drivers/display/DisplayBackend.h"

#include "modules/Buttons.h"
#include "modules/Display.h"
#include "modules/Printer.h"
#include "modules/Speaker.h"
#include "modules/Storage.h"

#include "Application.h"
#define MAX_APPLICATIONS 16
#define MAX_WIDGETS 16

#define WIDGET_HEIGHT
#define TAB_HEIGHT

class RecipOS {
public:
	RecipOS(void);
	DisplayBackend* displayBackend;
	Display* mainDisplay;
	Buttons* buttons;
	Printer* printer;
	Speaker* speaker;
	Storage* storage;

	// Applications
	Application* apps[MAX_APPLICATIONS];
	int addApplication(Application* app);
	int currentApp = -1;
	bool switchApp(int appid);
	bool tabLeft(void);
	bool tabRight(void);

	// Widgets
	Application* widgets[MAX_WIDGETS];
	int addWidget(Application* widget);
	int currentWidget = -1;

	bool boot(void);
	bool booted = false;

private:
	int nothing = 2;
};

#endif
