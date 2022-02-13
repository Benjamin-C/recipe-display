/*
 * RecipeSelector.cpp
 *
 *  Created on: Feb 12, 2022
 *      Author: benjamin
 */

#include "RecipeSelector.h"
#include "../recipos/RecipOS.h"
#include <string.h>
#include <stdio.h>
#include "RecipeUtils.h"

RecipeSelector::~RecipeSelector() {
	// TODO Auto-generated destructor stub
}

void RecipeSelector::startup(RecipOS* os) {
	this->os = os;
	strcpy((char*) name, "Test Application");
	strcpy((char*) abriv, "qqqq");
	color = EGA_BRIGHT_RED;
}

void RecipeSelector::onMessage(int mid, const char* dest, void* mbox) {

}

void RecipeSelector::paintTab(Display* d) {
	d->fill(egaColors[color]); return;
}

void RecipeSelector::runTab(void) {

}

void RecipeSelector::onButtonPress(uint16_t pressed, Buttons* buttons) {
	if((pressed & BUTTON_RIGHT_MASK) > 0) {
		os->tabRight();
		printf("Right\n");
	} else if((pressed & BUTTON_LEFT_MASK) > 0) {
		os->tabLeft();
		printf("Left\n");
	} else if((pressed & BUTTON_UP_MASK) > 0) {
		// Send example message
	}
}

void RecipeSelector::paintWidget(Display* d) {

}
void RecipeSelector::runService(void) {

}
