/* ButtonReader.cpp
 *
 * Reads the hardware buttons
 *
 * Author: Rhema Losli
 */

#include "ButtonReader.h"

// You should be able to compile some Arduino functions with this, but they may not work
#include "../../../arduinostandin.h"

/* To: Rhema
 *
 * Welcome to the world of OS writing with Ben (who probably shouldn't be allowed to write an OS but he does anyway)
 *
 * This source file will contain the source code for your button reading methods.
 * The higher level classes expect uint16_t ButtonReader::checkButtons(void) and bool ButtonReader::checkButton(int id)
 * 		to exist and work, so you must implement them.
 * You are welcome to implement any other methods you need. Don't forget to put them in ButtonReader.h as well as defining them here.
 */

ButtonReader::ButtonReader(void) {
	// TODO Rhema: Write this
	// Any init code you need. EX: pinMode(...
}

/* Gets the current state of all the buttons packed into a word
 *
 * With MSBF: bit 15 -> bit 0
 * Bit0: BUTTON_UP
 * Bit1: BUTTON_DOWN
 * Bit2: BUTTON_LEFT
 * Bit3: BUTTON_RIGHT
 * Bit4: BUTTON_ENTER
 * Bit5: BUTTON_SUPER
 *
 * EX: Up, Left, and Super are pressed: 00000000 00100101
 */
uint16_t ButtonReader::checkButtons(void) {
	// TODO Rhema: Write this
	return 0;
}
// Checks the status of a specific button. See the section in ButtonBackend.h for IDs.
bool ButtonReader::checkButton(int id) {
	// TODO Rhema: Write this
	return false;
}
