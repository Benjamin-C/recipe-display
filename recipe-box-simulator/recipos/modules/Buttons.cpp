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

const char* directions[7] = {"null", "up", "down", "left", "right", "enter", "super"};

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
	if(num <= 6 && num > 0) {
		out = 1 << (num-1);
		int oc = out;
		for(int i = 0; i < 8; i++) {
			printf("%01d", (oc >> 7) & 1);
			oc <<= 1;
		}
		printf("\nYou pressed %s\n", directions[num]);
		return out;
	} else {
		printf("Invalid button %d\n", num);
		return 0;
	}
}
