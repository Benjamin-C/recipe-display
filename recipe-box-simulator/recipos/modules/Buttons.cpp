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
#include <string.h>
#include <iostream>
#include <stdint.h>

// Needed to make simulator work
// You may also need to add -lpthread to your compiler options
#include <thread>
#include <atomic>
#include <memory>
#include <pthread.h>
#include <chrono>

const char* directions[7] = {"null", "up", "down", "left", "right", "enter", "super"};

Buttons::Buttons(void) {
	bb = NULL;
}

bool Buttons::checkButton(int id) {
	return false;
}

uint16_t Buttons::checkButtons() {
	if(bb != NULL) {
		return bb->checkButtons();
	} else {
		return 0;
	}
}

void Buttons::halt(void) {

}
