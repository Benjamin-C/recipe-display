/*
 * InstructionsTab.cpp
 *
 *  Created on: Feb 14, 2022
 *      Author: Rhema
 */

#include "PrintTab.h"
#include "../recipos/RecipOS.h"
#include <stdio.h>
#include <string.h>
#include "RecipeUtils.h"
#include <Arduino.h>

void PrintTab::startup(RecipOS* os) {
	this->os = os;
	strcpy(name, "Test Application");
	int i = 7;
	printf("Startup: %d\n", i);
	sprintf(abriv, "Prnt");
	color = EGA_BRIGHT_MAGENTA;
}
void PrintTab::onMessage(int mid, std::string dest, void* mbox) {
	if(mid != lastMID) {
		lastMID = mid;
		if(dest == "recipe-select") {
			ro = (RecipeUtils::Recipe*) mbox;
		}
	}
}
void PrintTab::paintTab(Display* d, bool repaint) {
	printf("color: %d %s\n", color, abriv);

	if(ro != NULL) {
		if(!repaint) {
			d->fill(BLACK);
			d->displayString(32,32,ro->name,2,egaColors[color],BLACK);
			d->displayString(32,48,"Press ENTER to print",2,egaColors[color],BLACK);
		}
	}
	else {
		printf("Recipe object is null :(\n");
		os->error("Recipe object is null :(\n");
	}
	return;
}
void PrintTab::runTab(void) {
	printf("App loaded! %d\n", color);
}
void PrintTab::onButtonPress(uint16_t pressed, Buttons* buttons) {
	if((pressed & BUTTON_RIGHT_MASK) > 0) {
		os->tabRight();
		printf("Right\n");
	}
	else if((pressed & BUTTON_LEFT_MASK) > 0) {
		os->tabLeft();
		printf("Left\n");
	}
	else if((pressed & BUTTON_ENTER_MASK) > 0) {
		// TODO print the recipe
	}
	os->sendMessage(os->getNextMID(), "testmsg", (void*) &pressed);
	printf("Someone pushed my button!\n");
	os->displayBackend->screenshot();
}

void PrintTab::runService(void) { }
void PrintTab::paintWidget(Display* d) { }
