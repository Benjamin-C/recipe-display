/*
 * Storage.cpp
 *
 *  Created on: Jan 11, 2022
 *      Author: benjamin
 */

#include <string.h>
#include "Storage.h"

Storage::Storage() {
	// TODO Auto-generated constructor stub

}

Storage::~Storage() {
	// TODO Auto-generated destructor stub
}

void Storage::readFile(char* path, char* dest, int start, int maxLength) {
	if(!strcmp(path, "/dev/null")) {
		return;
	} else if(!strcmp(path, "/dev/zero")) {
		for(int i = 0; i < maxLength; i++) {
			*(dest+i) = 0;
		}
	} else {
		switch(maxLength) {
		case 0: { } break;
		case 1: { *dest = 0; } break;
		case 2: { strcpy(dest, "E"); } break;
		case 3: { strcpy(dest, "ER"); } break;
		case 4:
		case 5: { strcpy(dest, "ERR"); } break;
		default: { strcpy(dest, "ERROR"); } break;
		}
	}
}

