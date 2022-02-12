/* RecipOS.h
 *
 * Operating system for the Recipe Box device
 *
 * This class handles most of the running of the box, including
 *   drawing windows, managing apps, running services, user input,
 *   outputs, and more.
 *
 * Author: Benjamin Crall
 */

// TODO improve comments

#ifndef RECIPOS_H
#define RECIPOS_H

#include "drivers/display/DisplayBackend.h"

#include "modules/Buttons.h"
#include "modules/Display.h"
#include "modules/Printer.h"
#include "modules/Speaker.h"
#include "modules/Storage.h"

#include "Application.h"

#define MAX_TABS 16
#define MAX_WIDGETS 16
#define MAX_SERVICES 16

#define WIDGET_HEIGHT 32
#define TAB_HEIGHT 16
#define NUM_TABS 6

class RecipOS {
public:
	// the main constructor. Call it to start a new OS. Not recommended to do multiple times, although it probably won't break
	RecipOS(void);
	// Call this when you are ready to start the OS. Once booted, the OS will take over the calling thread
	bool boot(void);
	// The DisplayBackend that all Display objects will point to
	DisplayBackend* displayBackend;
	// The main display that covers the entire screen. Don't use it unless you need full screen write privlages
	Display* mainDisplay;
	// The Buttons for button uses
	Buttons* buttons;
	// Not implemented
	Printer* printer;
	// Not implemented
	Speaker* speaker;
	// Not implemented
	Storage* storage;

	// ---------------------
	// Applications
	// ---------------------

	// Gets the next message ID for the next message
	int getNextMID(void); // rebuild me
	/* Sends a message to all applications.
	 * id holds a (hopefully) unique message ID
	 * dest holds a string stating the destination.
	 * box holds the message body.
	 */
	void sendMessage(int nid, const char* dest, void* msgbox);

	/* Adds a new Tab to the menu
	 * Currently, only 6 tabs will be shown
	 * Pass a pointer to your Application/TabApp,
	 *   and a void pointer to anything you want to set the Application's box variable to.
	 *   Passing NULL will leave the Application's box variable unchanged
	 */
	int addTab(TabApp* app, void* box); // Pass NULL to leave the box unchanged
	/* Switch to a tab by index
	 * Don't pass a number > 15 or < 0, it might break.
	 * TODO make sure it won't break
	 */
	bool switchTab(int appid);
	/* Switch to the next tab to the left, if it exists
	 */
	bool tabLeft(void);
	/* Switch to the next tab to the right, if it exists
	 */
	bool tabRight(void);
	/* Redraws the current tab
	 */
	bool repaintCurrentTab(void);

	// ---------------------
	// Widgets
	// ---------------------
	/* Adds a new Widget to the menu
	 * Currently, only 6 tabs will be shown
	 * Pass a pointer to your Application/WidgetApp,
	 *   and a void pointer to anything you want to set the Application's box variable to.
	 *   Passing NULL will leave the Application's box variable unchanged
	 */
	int addWidget(WidgetApp* widget, void* box); // Pass NULL to leave the box unchanged

	// ---------------------
	// Services
	// ---------------------
	/* Adds a new Service to the OS
	 * Currently, only 6 tabs will be shown
	 * Pass a pointer to your Application/ServiceApp,
	 *   and a void pointer to anything you want to set the Application's box variable to.
	 *   Passing NULL will leave the Application's box variable unchanged
	 */
	int addService(Application* widget, void* box); // Pass NULL to leave the box unchanged

private: // TODO add comments
	bool booted = false;

	bool checkButtonPress(void);

	// The next message ID to be assigned
	int nextMID = 0;

	// Applications
	TabApp* tabs[MAX_TABS];
	Display* tabDisplays[MAX_TABS];
	int currentTab = -1;
	bool drawTabList(void);
	Display* createTabDisplay(void);

	// Widgets
	Application* widgets[MAX_WIDGETS];
	Display* widgetDisplays[MAX_WIDGETS];
	int currentWidget = -1;
	bool drawWidgets(void);

	// Services
	Application* services[MAX_SERVICES];
	bool runServices(void);
};

#endif
