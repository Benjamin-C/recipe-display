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
	strcpy((char*) name, "Recipe Selector");
	strcpy((char*) abriv, "List");
	color = EGA_BRIGHT_MAGENTA;

	optionCount = 8;
	options = new std::string[8];
	options[0] = "Chocolate Cake";
	options[1] = "Raspberry Filet";
	options[2] = "BBQ Lasagne";
	options[3] = "Deep-Dish Ravioli";
	options[4] = "Moist Butter";
	options[5] = "Potato Jam";
	options[6] = "Deep-fried Okra";
	options[7] = "Burnt Toast";

	currentSelection = 3;
}

void RecipeSelector::onMessage(int mid, std::string dest, void* mbox) {
	if(dest == "recipe-select") {
		os->error("You can't select recipies yet!");
	}
}

void RecipeSelector::paintTab(Display* d) {
	d->fill(SELECTOR_BACKGROUND_COLOR);
	int starty = 0;
	for(int i = 0; i < optionCount; i++) {
		int fore = SELECTOR_TEXT_COLOR;
		int back = SELECTOR_BACKGROUND_COLOR;
		if(i == currentSelection) {
			fore = SELECTOR_BACKGROUND_COLOR;
			back = SELECTOR_TEXT_COLOR;
			d->fillRect(options[i].length()*16, starty, d->width()-options[i].length()*16, 16, back);
		}
		d->displayString(0, starty, options[i], 2, fore, back);
		starty += 16;
	}

	return;
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
		if(currentSelection > 0) {
			currentSelection--;
			os->repaintCurrentTab();
		}
	} else if((pressed & BUTTON_DOWN_MASK) > 0) {
		if(currentSelection < optionCount - 1) {
			currentSelection++;
			os->repaintCurrentTab();
		}
	} else if((pressed & BUTTON_ENTER_MASK) > 0) {
		printf("Messaging...");
		// Should put the recipe here, but doesn't yet
		os->sendMessage(os->getNextMID(), "recipe-select", (void*) &currentSelection);
	}

	os->displayBackend->screenshot();
}

void RecipeSelector::paintWidget(Display* d) {

}
void RecipeSelector::runService(void) {

}
