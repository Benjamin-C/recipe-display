/*
 * buttons.cpp
 *
 *  Created on: Jan 11, 2022
 *      Author: benjamin
 */

// TODO write methods
// TODO add comments

#include "Buttons.h"
#include <stdio.h>

Buttons::Buttons(void) {

}

bool Buttons::checkButton(int id) {
	return false;
}

uint16_t Buttons::checkButtons() {
	printf("What button do you want to press? ");
	int num = 0;
	scanf("%1d", &num);
	uint16_t out;
	if(num <= 6) {
		out = 1 << num;
		printf("You pressed %d\n", num);
	}
	return out;
	return 7;
}
