/* ButtonReader.h
 *
 * Reads the hardware buttons
 *
 * Author: Rhema Losli
 */

#ifndef BUTTON_READER_H
#define BUTTON_READER_H

#include <stdint.h>

#include "ButtonBackend.h"

class ButtonReader : public ButtonBackend {
public:
	ButtonReader(void);
	// Gets the current state of all the buttons packed into a word
	uint16_t checkButtons(void);
	// Checks the status of a specific button. See the section above for IDs.
	bool checkButton(int id);

};

#endif
