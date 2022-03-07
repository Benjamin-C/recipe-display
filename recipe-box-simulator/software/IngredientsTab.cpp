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
#include <iostream>
#include <iomanip>
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
		if(dest == "recipe-select") {
			// Get the recipe from the box
			// The box is a pointer to something, and we know that that something
			// is a recipe structure, so tell the compiler that
			ro = (RecipeUtils::Recipe*) mbox;
			// Use the recipe for whatever you want
			//RecipeUtils::printRecipe(ro);
		}
	}
}
void IngredientsTab::paintTab(Display* d) {
	printf("color: %d %s\n", color, abriv);
	d->fill(egaColors[color]);
	d->displayString(8,0,ro->name,2,WHITE,egaColors[color]);

	int starty = 32;

	if(ro != NULL) {
		for(int i = currentSelection; i < ro->ingredientCount; i++) {
			float amount = ro->ingredients[i].amount;
			std::string str = std::to_string(amount);
			if(str.find('.') != std::string::npos){
				str = str.substr(0, str.find_last_not_of('0')+1);
				if(str.find('.') == str.size()-1){
				  str = str.substr(0, str.size()-1);
				}
			}

			std::string unit;

			switch(ro->ingredients[i].unit) {
			case RecipeUtils::PIECE: unit = "piece"; break;
			case RecipeUtils::CUP: unit = "cup"; break;
			case RecipeUtils::TABLESPOON: unit = "tbsp"; break;
			case RecipeUtils::TEASPOON: unit = "tsp"; break;
			case RecipeUtils::NOTHING: unit = "nothing"; return; // Don't print s for plural
			}
			if(ro->ingredients[i].amount != 1) {
				unit = unit + "s";
			}

			d->displayString(0,starty,str,2,WHITE,egaColors[color]);
			d->displayString(72,starty,unit,2,WHITE,egaColors[color]);

			int r = d->displayWrappedString(184, starty, ro->ingredients[i].name, 18, 0, 2, WHITE, egaColors[color]);
			starty += r * 16;
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
			if(currentSelection < ro->ingredientCount - 1) {
				currentSelection++;
				os->repaintCurrentTab();
			}
		}
	}
	os->sendMessage(os->getNextMID(), "testmsg", (void*) &pressed);
	printf("Someone pushed my button!\n");
	os->displayBackend->screenshot();
}

void IngredientsTab::runService(void) { }
void IngredientsTab::paintWidget(Display* d) { }
