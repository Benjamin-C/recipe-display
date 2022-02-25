/*
 * IngredientsTab.h
 *
 *  Created on: Feb 25, 2022
 *      Author: Rhema
 */

#ifndef SOFTWARE_INGREDIENTSTAB_H_
#define SOFTWARE_INGREDIENTSTAB_H_

#include "../recipos/RecipOS.h"

class IngredientsTab : public Application {
public:
	~IngredientsTab() { };
	void startup(RecipOS* os);
	void onMessage(int mid, std::string dest, void* mbox);
	void paintTab(Display* d);
	void runTab(void);
	void onButtonPress(uint16_t pressed, Buttons* buttons);

	void paintWidget(Display* d);
	void runService(void);
private:
	int lastMID = -1;
};
#endif
