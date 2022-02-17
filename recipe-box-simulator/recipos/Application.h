/*
 * Application.h
 *
 * Application interface definition.
 *
 * Each application can be used as a tab, a service, and a widget.
 * Define the appropriate methods for each option.
 *
 * The app must be registered with the OS for each app type it is intended to run as
 *
 * Tabs:
 *  - Draw in the main area of the screen
 *  - Only one active at a time
 *  - Should be the only part to respond to buttons
 *
 * Widgets:
 *  - Drawn in a bar at the top of the screen
 *  - Always visible at the top of the screen
 *  - Should not respond to buttons
 *
 * Services:
 *  - Invisible
 *  - Runs at an interval
 *  - Currently runs on main OS thread
 *  - Should be short tasks to not lag the OS too much
 *
 *  Created on: Jan 12, 2022
 *      Author: benjamin
 */

#ifndef RECIPOS_APPLICATION_H_
#define RECIPOS_APPLICATION_H_

//class RecipOS;
#include "RecipOS.h"
#include "modules/Display.h"

#define MAX_NAME_LENGTH 32
#define MAX_ABRIV_LENGTH 4

class RecipOS;

class Application {
public:
	virtual ~Application() { };
	/* Called once when the app starts
	 * Should initialize any variables needed, although should not re-initialize
	 *   any variables that are externally set
	 * Will only be called on the first time the app is registered as a tab, widget, or service
	 */
	virtual void startup(RecipOS* os) = 0;
	// The full name of the application
	char name[MAX_NAME_LENGTH+1];
	// the <=5 character abbreviation of the app
	char abriv[MAX_ABRIV_LENGTH+1];
	/* The color used for the tab color (if present)
	 * This is the EGA color number, not the 16bit color number
	 */
	uint8_t color;
	// If the app is running, should not be modified by the app
	bool running = false;
	// The pointer to the OS that the app is running under
	RecipOS* os;
	/* A user-defined storage box
	 * The value can be passed in when the Application is registered to the OS, or modified thereafter
	 */
	void* box;

	/* Receives messages from other applications.
	 * Every application receives every message, the app must use dest to determine
	 *   if the message is intended for that app.
	 * Gets the message ID, a string dest and a pointer to the message box.
	 */
	virtual void onMessage(int mid, std::string dest, void* mbox) = 0;

	// ------------------------------
	// Tab Methods
	// ------------------------------

	/* Called when the app should draw itself as a tab
	 * Gets a pointer to the main display area
	 */
	virtual void paintTab(Display* d) = 0;
	/* Called when the tab is selected before paintTab
	 * paintTab may be called without calling runTab first
	 */
	virtual void runTab(void) = 0;
	/* Called when a button is pressed
	 * Only called for the app with the active tab
	 */
	virtual void onButtonPress(uint16_t pressed, Buttons* buttons) = 0;

	// ------------------------------
	// Widget Methods
	// ------------------------------

	/* The desired width of the widget in px.
	 * The real width may differ, check with d->width()
	 * Values < 1 will not draw
	 */
	int widgetWidthRequest = -1;
	/* Called when the app should draw itself as a widget
	 * Gets a pointer to the display for the widget area
	 */
	virtual void paintWidget(Display* d) = 0;
	/* Called when the tab is selected
	 * Called just before paintTab
	 */

	// ------------------------------
	// Service Methods
	// ------------------------------

	/* Requested run interval in ms
	 * The real interval will vary, check times with millis()
	 * Values < 0 will not run as a service
	 * Values == 0 will run as often as possible;
	 */
	long serviceInterval = -1;
	/* The next time the service should run
	 * Automatically updated when the service runs
	 * Intervals may be skipped if the system is running slow
	 * Do not modify this unless you know what you're doing
	 */
	unsigned long nextServiceTime;
	/* Called when the service runs
	 * The interval will not be exactly the requested time,
	 *   and may skip intervals if the sysetm is running slow
	 * Check millis() if precise timing is required.
	 */
	virtual void runService(void) = 0;

};

// More descriptive names since Applications are used in may places
typedef Application TabApp;
typedef Application WidgetApp;
typedef Application ServiceApp;

/*

// An example of a minimal app
class ExampleApp : public Application {
public:
	~ExampleApp() { };
	void startup(RecipOS* os) {
		this->os = os;
		strcpy((char*) name, "Test Application");
		strcpy((char*) abriv, "qqqq");
		color = EGA_BRIGHT_RED;
	}
	void onMessage(int mid, const char* dest, void* mbox) { }
	void paintTab(Display* d) {
		d->fill(egaColors[color]); return;
	}
	void runTab(void) { }
	void onButtonPress(uint16_t pressed, Buttons* buttons) { }

	void paintWidget(Display* d) { }
	void runService(void) { }
};

*/

#endif /* RECIPOS_APPLICATION_H_ */
