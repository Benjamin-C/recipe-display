/*
 * InstructionsTab.cpp
 *
 *  Created on: Feb 14, 2022
 *      Author: Rhema
 */

#include "InstructionsTab.h"
#include "../recipos/RecipOS.h"
#include <stdio.h>
#include <string.h>
#include "RecipeUtils.h"

void InstructionsTab::startup(RecipOS* os) {
	this->os = os;
	strcpy(name, "Test Application");
	int i = 7;
	printf("Startup: %d\n", i);
	sprintf(abriv, "Inst");
	color = i+3;
}
void InstructionsTab::onMessage(int mid, std::string dest, void* mbox) {
	if(mid != lastMID) {
		lastMID = mid;
		if(dest == "recipe-select") {
			ro = (RecipeUtils::Recipe*) mbox;
		}
	}
}
void InstructionsTab::paintTab(Display* d) {
	printf("color: %d %s\n", color, abriv);
	d->fill(egaColors[color]);
	d->displayString(8,0,ro->name,2,WHITE,egaColors[color]);


	int starty = 32;

	if(ro != NULL) {
		for(int i = currentSelection; i < ro->stepCount; i++) {
			printf("%s\n", ro->steps[i].text.c_str());
			printf("%d\n", ro->steps[i].number);

			d->displayString(0,starty,std::to_string(ro->steps[i].number)+".",2,WHITE,egaColors[color]);

			int r = d->displayWrappedString(32, starty, ro->steps[i].text, 27, 0, 2, WHITE, egaColors[color]);
			starty += r * 16;
			if(starty >= d->height()) {
				break;
			}
		}
	}
	else {
		printf("Recipe object is null :(\n");
		os->error("Recipe object is null :(\n");
	}
	return;
}
void InstructionsTab::runTab(void) {
	printf("App loaded! %d\n", color);
}
void InstructionsTab::onButtonPress(uint16_t pressed, Buttons* buttons) {
	if((pressed & BUTTON_RIGHT_MASK) > 0) {
		os->tabRight();
		printf("Right\n");
	}
	else if((pressed & BUTTON_LEFT_MASK) > 0) {
		os->tabLeft();
		printf("Left\n");
	}
	else if((pressed & BUTTON_UP_MASK) > 0) {
		if(currentSelection > 0) {
			currentSelection--;
			os->repaintCurrentTab();
		}
	}
	else if((pressed & BUTTON_DOWN_MASK) > 0) {
		if(ro != NULL) {
			if(currentSelection < ro->stepCount - 1) {
				currentSelection++;
				os->repaintCurrentTab();
			}
		}
	}
	os->sendMessage(os->getNextMID(), "testmsg", (void*) &pressed);
	printf("Someone pushed my button!\n");
	os->displayBackend->screenshot();
}

void InstructionsTab::runService(void) { }
void InstructionsTab::paintWidget(Display* d) { }
