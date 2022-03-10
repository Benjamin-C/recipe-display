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
#include <string>
#include "RecipeUtils.h"
#include <Arduino.h>

void IngredientsTab::startup(RecipOS* os) {
	this->os = os;
	strcpy(name, "Test Application");
	int i = 6;
	printf("Startup: %d\n", i);
	sprintf(abriv, "Ingr");
	color = EGA_YELLOW;
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
			currentSelection = 0;
			mayScrollDown = false;
		}
	}
}
void IngredientsTab::paintTab(Display* d, bool repaint) {
	if(ro != NULL) {
		if(!repaint) {
//			d->fill(BLACK);
			d->fillRect(0, 0, d->width(), 32, BLACK);
			d->displayString(8,0,ro->name,2,egaColors[color],BLACK);
		}

		int starty = 32;

		if(currentSelection > 0) {
			d->displayChar(0, 16, 0x1E, 2, egaColors[color],BLACK);
		} else {
			d->fillRect(0, 16, 16, 16, BLACK);
		}

		int foundEnd = -1;

		for(int i = currentSelection; i < ro->ingredientCount; i++) {
			float amount = ro->ingredients[i].amount;
			std::string str = std::string(String(amount).c_str());

//			std::string str = "abc"; // TODO fix this
			if(str.find('.') != std::string::npos){
				str = str.substr(0, str.find_last_not_of('0')+1);
				if(str.find('.') == str.size()-1){
				  str = str.substr(0, str.size()-1);
				}
			}

			std::string unit;

			switch(ro->ingredients[i].unit) {
			case RecipeUtils::PIECE: unit = "piece  "; break;
			case RecipeUtils::CUP: unit = "cup    "; break;
			case RecipeUtils::TABLESPOON: unit = "tbsp   "; break;
			case RecipeUtils::TEASPOON: unit = "tsp   "; break;
			case RecipeUtils::NOTHING: unit = "HELP  "; break; // Don't print s for plural
			}
			if(ro->ingredients[i].amount != 1) {
				unit = unit + "s";
			}

			d->displayPaddedString(0,starty,str,5,2,egaColors[color],BLACK);
			d->displayPaddedString(5*16,starty,unit,7,2,egaColors[color],BLACK);

//			printf("Printing stringy thingy %s %s %s\n", str.c_str(), unit.c_str(), ro->ingredients[i].name.c_str());
			int r = d->displayWrappedString((5+7)*16, starty, ro->ingredients[i].name, 18, 0, 2, egaColors[color],BLACK);
			d->fillRect(0, starty+16, (5+7)*16, (r-1) * 16, RED);
			starty += r * 16;
			if(starty >= d->height()) {
				foundEnd = i;
				printf("Found end of screen, so stopped\n");
				break;
			}
		}
		if(foundEnd >= 0 && foundEnd < ro->ingredientCount - 1) {
//			printf("%d %d\n", currentSelection, ro->ingredientCount - 1);
			d->displayChar(16, 16, 0x1F, 2, egaColors[color],BLACK);
			mayScrollDown = true;
		} else {
			d->fillRect(16, 16, 16, 16, BLACK);
			mayScrollDown = false;
		}
		d->fillRect(0, starty, d->width(), d->height()-starty, BLACK);
	}
	else {
//		d->fill(BLACK);
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
			if(currentSelection < ro->ingredientCount - 1 && mayScrollDown) {
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
