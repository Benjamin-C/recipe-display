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
//#include <thread>
#include <atomic>
//#include <memory>
//#include <pthread.h>
//#include <chrono>

#include <Arduino.h>

const char* directions[7] = {"null", "up", "down", "left", "right", "enter", "super"};
std::atomic<int> buttons;

// This is running on Core 0, everything else is on Core 1.
// Why does this freeze when big things are going on on Core 1?
static void Task1code( void * parameter) {
	if(parameter != NULL) {
		ButtonBackend* bb = (ButtonBackend*) parameter;
		while(true) {
			if(buttons == 0) {
				int lastButtons = buttons;
				buttons = bb->checkButtons();
				if(buttons != 0 && buttons != lastButtons) {
					int b = buttons;
//					printf("Got Button! %d %d =======================\n", xPortGetCoreID(), b);
				}
//				printf("Check Buttons %ld\n", millis());
				delay(50);
			}
		}
	} else {
		printf("BB was null!");
	}
}

Buttons::Buttons(void) {
	bb = NULL;
	Task1 = NULL;
}

bool Buttons::start(ButtonBackend* bb) {
	if(bb != NULL) {
		xTaskCreatePinnedToCore(
				Task1code, /* Function to implement the task */
			  "Task1", /* Name of the task */
			  10000,  /* Stack size in words */
			  bb,  /* Task input parameter */
			  0,  /* Priority of the task */
			  &Task1,  /* Task handle. */
			  0); /* Core where the task should run */
		return true;
	} else {
		return false;
	}
}

bool Buttons::checkButton(int id) {
	return false;
}

uint16_t Buttons::checkButtons() {
//	printf("%d\n", xPortGetCoreID());
	int tb = buttons;
	buttons = 0;
	return tb;
//	if(bb != NULL) {
//		return bb->checkButtons();
//	} else {
//		return 0;
//	}
}

void Buttons::halt(void) {
	vTaskDelete(Task1);
}
