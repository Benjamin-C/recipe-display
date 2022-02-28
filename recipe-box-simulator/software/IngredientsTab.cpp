/*
 * IngredientsTab.cpp
 *
 *  Created on: Feb 25, 2022
 *      Author: Rhema
 */

#include "IngredientsTab.h"
#include "../recipos/RecipOS.h"
#include <stdio.h>
#include <string.h>
#include "RecipeUtils.h"

void IngredientsTab::startup(RecipOS* os) {
	this->os = os;
	strcpy(name, "Test Application");
	int i = 6;
	printf("Startup: %d\n", i);
	sprintf(abriv, "Ingr");
	color = i+3;
}
void IngredientsTab::onMessage(int mid, std::string dest, void* mbox) {
	if(mid != lastMID) {
		lastMID = mid;
		if(dest == "testmsg") {
			printf("Button press message! %d\n", *((uint16_t*) mbox));
		}
	}
}
void IngredientsTab::paintTab(Display* d) {
	printf("color: %d %s\n", color, abriv);
	d->fill(egaColors[color]);
	d->displayString(0,0,name,2,WHITE,egaColors[color]);

	if(ro == NULL) {
		for(int i = 0; i < ro->ingredientCount; i++) {
			d->displayString(0,18*i+18,ro->ingredients[i].name,2,WHITE,egaColors[color]);
		}
	}
	else {
		printf("Recipe object is null :(\n");
		os->error("Recipe object is null :(\n");
	}
	return;
}
void IngredientsTab::runTab(void) {
	printf("App loaded! %d\n", color);
}
void IngredientsTab::onButtonPress(uint16_t pressed, Buttons* buttons) {
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

void IngredientsTab::runService(void) { }
void IngredientsTab::paintWidget(Display* d) { }
