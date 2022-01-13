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

class Application {
public:
	virtual ~Application() { };
	virtual void startup(void) = 0;
	virtual void paint(Display* d) = 0;
	const char* name;
	const char* abriv;
	bool running = false;
};

#endif /* RECIPOS_APPLICATION_H_ */
