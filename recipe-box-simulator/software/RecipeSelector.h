/*
 * RecipeSelector.h
 *
 *  Created on: Feb 12, 2022
 *      Author: benjamin
 */

#ifndef SOFTWARE_RECIPESELECTOR_H_
#define SOFTWARE_RECIPESELECTOR_H_

#include "../recipos/RecipOS.h"
#include "RecipeUtils.h"

#define SELECTOR_BACKGROUND_COLOR BLACK
#define SELECTOR_TEXT_COLOR BRIGHT_CYAN

class RecipeSelector : public Application {
public:
	~RecipeSelector();
	void startup(RecipOS* os);
	void onMessage(int mid, std::string dest, void* mbox);
	void paintTab(Display* d, bool repaint);
	void runTab(void);
	void onButtonPress(uint16_t pressed, Buttons* buttons);

	void paintWidget(Display* d);
	void runService(void);
private:
	int currentSelection;
	int optionCount;
	std::string* optionNames;
	std::string* options;
};

#endif
