/*
 * InstructionsTab.h
 *
 *  Created on: Feb 14, 2022
 *      Author: Rhema
 */

#ifndef SOFTWARE_INSTRUCTIONSTAB_H_
#define SOFTWARE_INSTRUCTIONSTAB_H_

#include "../recipos/RecipOS.h"

class InstructionsTab : public Application {
public:
	~InstructionsTab() { };
	void startup(RecipOS* os);
	void onMessage(int mid, const char* dest, void* mbox);
	void paintTab(Display* d);
	void runTab(void);
	void onButtonPress(uint16_t pressed, Buttons* buttons);

	void paintWidget(Display* d);
	void runService(void);
private:
	int lastMID = -1;
};
#endif
