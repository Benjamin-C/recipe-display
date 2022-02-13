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

class RecipeSelector : public Application {
public:
	~RecipeSelector();
	void startup(RecipOS* os);
	void onMessage(int mid, const char* dest, void* mbox);
	void paintTab(Display* d);
	void runTab(void);
	void onButtonPress(uint16_t pressed, Buttons* buttons);

	void paintWidget(Display* d);
	void runService(void);
};

#endif
