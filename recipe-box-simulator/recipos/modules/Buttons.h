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

#include "../drivers/buttons/ButtonBackend.h"

extern const char* directions[7];

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
