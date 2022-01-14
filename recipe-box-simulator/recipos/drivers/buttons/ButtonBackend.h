/* ButtonBackend.h
 *
 * Interface for reading buttons
 *
 * Author: Benjamin Crall
 */

#ifndef BUTTON_BACKEND_H
#define BUTTON_BACKEND_H

#include <stdint.h>

// The IDs of the buttons to use with checkButtons.
// Their bit when packed will be their ID - 1
// All buttons except super may be mutually exclusive.
#define BUTTON_NULL 0 // No button
#define BUTTON_UP 1 // up arrow
#define BUTTON_DOWN 2 // down arrow
#define BUTTON_LEFT 3 // left arrow
#define BUTTON_RIGHT 4 // right arrow
#define BUTTON_ENTER 5 // enter button
#define BUTTON_SUPER 6 // special button for whatever you want.

class ButtonBackend {
public:
	virtual ~ButtonBackend() { };
	// Gets the current state of all the buttons packed into a word
	virtual uint16_t checkButtons(void) = 0;
	// Checks the status of a specific button. See the section above for IDs.
	virtual bool checkButton(int id) = 0;
};

#endif

