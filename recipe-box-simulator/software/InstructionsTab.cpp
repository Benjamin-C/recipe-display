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

			d->displayString(0,18*i+36,std::to_string(ro->steps[i].number)+".",2,WHITE,egaColors[color]);

			int leftx = 3*16;
			const char* c = ro->steps[i].text.c_str();
			bool hasMoreMsg = true;
			const int SCALE = 2;
			int rows = 0;
			int ystart = 4*16;
			while(rows < 12) {
				int xpos = leftx;
				int charCount = 0;
				d->displayChar(xpos, ystart, '\xBA', SCALE,WHITE,egaColors[color]);
				xpos += 8*SCALE;
				d->displayChar(xpos, ystart, ' ', SCALE,WHITE,egaColors[color]);
				xpos += 8*SCALE;
				const int LINEWIDTH = 20;
				int ll = 0;
				int tll = 0;
				const char* tc = c;
				while(tll < LINEWIDTH) {
					while(*tc != ' ' && *tc != '\0' && *tc != '\n') { tc++; tll++; }
					if(tll+1 < LINEWIDTH) {
						ll = tll;
						if(*tc != ' ') {
							break;
						} else {
							tc++;
							tll++;
						}
					}
				}
				while(charCount < LINEWIDTH) {
					if(*c != '\0' && *c != '\n' && ll > 0) {
						d->displayChar(xpos, ystart, *c, SCALE,WHITE,egaColors[color]);
						printf("%c", *c);
						c++;
						ll--;
						if(ll == 0 && *c != '\0') {
							c++;
						}
					} else {
						if(*c == '\0') {
							hasMoreMsg = false;
						}
						d->displayChar(xpos, ystart, ' ', SCALE,WHITE,egaColors[color]);
					}
					xpos += 8*SCALE;
					charCount++;
				}
				if(*c == '\n') {
					c++;
				}
				d->displayChar(xpos, ystart, ' ', SCALE,WHITE,egaColors[color]);
				xpos += 8*SCALE;
				d->displayChar(xpos, ystart, '\xBA', SCALE,WHITE,egaColors[color]);
				ystart += 8*SCALE;
				if(hasMoreMsg) {
					printf("\n");
				}
				rows++;
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
