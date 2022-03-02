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

	FileList allFiles = os->storage->getDirectoryList("./");

	optionCount = allFiles.count;
	options = new std::string[allFiles.count];
	optionNames = new std::string[allFiles.count];

	for(int i = 0; i < allFiles.count; i++) {
		std::string filename = allFiles.list[i];
		std::string file = os->storage->readFile(filename);
		RecipeUtils::Recipe* r = RecipeUtils::parseWithOptions(file, false, false);
		options[i] = filename;
		optionNames[i] = r->name;
	}

	currentSelection = 3;
}

void RecipeSelector::onMessage(int mid, std::string dest, void* mbox) {
	// This should be removed once the recipe viewers are ready
	// EXAMPLE: If the message is a recipe select message
//	if(dest == "recipe-select") {
//		// Get the recipe from the box
//		// The box is a pointer to something, and we know that that something
//		// is a recipe structure, so tell the compiler that
//		RecipeUtils::Recipe* rp = (RecipeUtils::Recipe*) mbox;
//		// Use the recipe for whatever you want
//		os->error("You can't select recipes yet!\n \n" + rp->name);
//		RecipeUtils::printRecipe(rp);
//	}
	// Possibly recipe viewer should register itself then report here to mention its id
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
			d->fillRect(optionNames[i].length()*16, starty, d->width()-optionNames[i].length()*16, 16, back);
		}
		d->displayString(0, starty, optionNames[i], 2, fore, back);
		starty += 16;
	}

	return;
}

void RecipeSelector::runTab(void) {

}

void RecipeSelector::onButtonPress(uint16_t pressed, Buttons* buttons) {
	using namespace RecipeUtils;
	printf("Button!");
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
		Recipe* rsp = RecipeUtils::parseString(os->storage->readFile(options[currentSelection]));
		os->sendMessage(os->getNextMID(), "recipe-select", (void*) rsp);
	}

	os->displayBackend->screenshot();
}

void RecipeSelector::paintWidget(Display* d) {

}
void RecipeSelector::runService(void) {

}
