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

std::atomic<uint16_t> buttons;
std::atomic<bool> mayRun;

uint16_t waitForButtons(void) {
	while(mayRun) {
		printf("What button do you want to press? ");
		int num = 0;
		scanf("%1d", &num);
		if(mayRun) {
			std::cin.ignore();
			uint16_t out;
			if(num <= 6 && num > 0) {
				out = 1 << (num-1);
				int oc = out;
				for(int i = 0; i < 8; i++) {
					printf("%01d", (oc >> 7) & 1);
					oc <<= 1;
				}
				printf("\nYou pressed %s\n", directions[num]);
				buttons = out;
			} else {
				printf("Invalid button %d\n", num);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		} else {
			break;
		}
	}
	return 0;
}



Buttons::Buttons(void) {
	mayRun = true;
	std::thread worker(waitForButtons);
	worker.detach();
}

bool Buttons::checkButton(int id) {
	return false;
}

uint16_t Buttons::checkButtons() {
	uint16_t tmp = buttons;
	buttons = 0;
	return (uint16_t) tmp;
}

void Buttons::halt(void) {
	mayRun = false;
}
