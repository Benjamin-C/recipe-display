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
#include <Arduino.h>

void InstructionsTab::startup(RecipOS* os) {
	this->os = os;
	strcpy(name, "Test Application");
	int i = 7;
	printf("Startup: %d\n", i);
	sprintf(abriv, "Inst");
	color = EGA_BRIGHT_GREEN;
}
void InstructionsTab::onMessage(int mid, std::string dest, void* mbox) {
	if(mid != lastMID) {
		lastMID = mid;
		if(dest == "recipe-select") {
			ro = (RecipeUtils::Recipe*) mbox;
			currentSelection = 0;
			mayScrollDown = false;
		}
	}
}
void InstructionsTab::paintTab(Display* d, bool repaint) {
	printf("color: %d %s\n", color, abriv);

	if(ro != NULL) {
		if(!repaint) {
//			d->fill(BLACK);
			d->fillRect(0, 0, d->width(), 32, BLACK);
			d->displayString(8,0,ro->name,2,egaColors[color],BLACK);
		}

		if(currentSelection > 0) {
			d->displayChar(0, 16, 0x1E, 2, egaColors[color],BLACK);
		} else {
			d->fillRect(0, 16, 16, 16, BLACK);
		}

		int foundEnd = -1;

		int starty = 32;

		for(int i = currentSelection; i < ro->stepCount; i++) {
			printf("%s\n", ro->steps[i].text.c_str());
			printf("%d\n", ro->steps[i].number);

			std::string str = std::string(String(ro->steps[i].number).c_str());
			d->displayPaddedString(0,starty,str+".",3,2,egaColors[color],BLACK);

			int r = d->displayWrappedString(48, starty, ro->steps[i].text, 27, 0, 2,egaColors[color],BLACK);
			d->fillRect(0, starty+16, 48, (r-1) * 16, BLACK);
			starty += r * 16;
			if(starty >= d->height()) {
				foundEnd = i;
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
			if(currentSelection < ro->stepCount - 1 && mayScrollDown) {
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
