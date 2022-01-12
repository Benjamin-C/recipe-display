/*
 * buttons.h
 *
 *  Created on: Jan 11, 2022
 *      Author: benjamin
 */

#ifndef RECIPOS_MODULES_BUTTONS_H_
#define RECIPOS_MODULES_BUTTONS_H_

#include <stdint.h>

#define BUTTON_NULL 0
#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_ENTER 5
#define BUTTON_SUPER 6

class Buttons {
public:
	Buttons(void);
	uint16_t checkButtons(void);
	bool checkButton(int id);
};

#endif /* RECIPOS_MODULES_BUTTONS_H_ */
