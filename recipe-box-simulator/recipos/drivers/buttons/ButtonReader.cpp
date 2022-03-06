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
	pinMode(BUTTON_PIN, INPUT);
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
	int bn = readAnalogButton();
	if(bn > 0) {
		printf("%d\n", bn);
	}
	return (bn > 0) ? 1 << bn-1 : 0;
}
// Checks the status of a specific button. See the section in ButtonBackend.h for IDs.
bool ButtonReader::checkButton(int id) {
	// TODO Rhema: Write this
	// Or don't, we don't really need it right now
	return false;
}

int ButtonReader::readAnalogButton(void) {
  int button = analogRead(BUTTON_PIN);
  // These exact values should probably be in a calibration table somewhere.
  // They are not. Too bad.
  if (button < 100) return 1; // up
  if (button < 2000) return 3; // left
  if (button < 2750) return 2; // Down
  if (button < 3050) return 4; // Right
  if (button < 3400) return 5; // Enter
  return 0;
}
