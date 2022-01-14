/*
 * Application.h
 *
 *  Created on: Jan 12, 2022
 *      Author: benjamin
 */

#ifndef RECIPOS_APPLICATION_H_
#define RECIPOS_APPLICATION_H_

//class RecipOS;
#include "RecipOS.h"
#include "modules/Display.h"

#define MAX_NAME_LENGTH 32
#define MAX_ABRIV_LENGTH 5

class Application {
public:
	virtual ~Application() { };
	virtual void startup(void) = 0;
	virtual void paint(Display* d) = 0;
	char name[MAX_NAME_LENGTH+1];
	char abriv[MAX_ABRIV_LENGTH+1];
	uint8_t color; // the EGA color number, not the 16bit color number
	bool running = false;
};

#endif /* RECIPOS_APPLICATION_H_ */
