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
	d->displayString(0,0,name,2,WHITE,egaColors[color]);

	if(ro != NULL) {
		for(int i = 0; i < ro->ingredientCount; i++) {
			printf("%s\n", ro->steps[i].text.c_str());
			printf("%d\n", ro->steps[i].number);

			d->displayString(32,18*i+36,ro->steps[i].text,2,WHITE,egaColors[color]);
			d->displayString(8,18*i+36,std::to_string(ro->steps[i].number),2,WHITE,egaColors[color]);
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
	} else if((pressed & BUTTON_LEFT_MASK) > 0) {
		os->tabLeft();
		printf("Left\n");
	}
	os->sendMessage(os->getNextMID(), "testmsg", (void*) &pressed);
	printf("Someone pushed my button!\n");
	os->displayBackend->screenshot();
}

void InstructionsTab::runService(void) { }
void InstructionsTab::paintWidget(Display* d) { }
