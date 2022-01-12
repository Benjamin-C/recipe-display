/* buttons.h
 *
 * Button module for RecipOS
 * Makes it easy for software to get the current state of the buttons.
 * Uses the hardware button driver to interface with the buttons
 *
 *  Author: Benjamin Crall
 */

#ifndef RECIPOS_MODULES_BUTTONS_H_
#define RECIPOS_MODULES_BUTTONS_H_

#include <stdint.h>

// The IDs of the buttons to use with checkButtons.
// All buttons except super may be mutually exclusive.
#define BUTTON_NULL 0 // No button
#define BUTTON_UP 1 // up arrow
#define BUTTON_DOWN 2 // down arrow
#define BUTTON_LEFT 3 // left arrow
#define BUTTON_RIGHT 4 // right arrow
#define BUTTON_ENTER 5 // enter button
#define BUTTON_SUPER 6 // special button for whatever you want.

// TODO implement buttons
// WARNIGN this is not yet implemented
class Buttons {
public:
	// INIT
	Buttons(void);
	// Gets the current state of all the buttons packed into a word
	uint16_t checkButtons(void);
	// Checks the status of a specific button. See the section above for IDs.
	bool checkButton(int id);
};

#endif /* RECIPOS_MODULES_BUTTONS_H_ */
