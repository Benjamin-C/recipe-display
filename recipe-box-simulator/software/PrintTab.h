/*
 * PrintTab.h
 *
 *  Created on: Feb 14, 2022
 *      Author: Rhema
 */

#ifndef SOFTWARE_PRINTTAB_H_
#define SOFTWARE_PRINTTAB_H_

#include "../recipos/RecipOS.h"
#include "RecipeUtils.h"

class PrintTab : public Application {
public:
	~PrintTab() { };
	void startup(RecipOS* os);
	void onMessage(int mid, std::string dest, void* mbox);
	void paintTab(Display* d, bool repaint);
	void runTab(void);
	void onButtonPress(uint16_t pressed, Buttons* buttons);

	void paintWidget(Display* d);
	void runService(void);
private:
	int lastMID = -1;
	RecipeUtils::Recipe* ro;
};
#endif
