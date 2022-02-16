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

void Storage::readFile(const char* path, char* dest, int start, int maxLength) {
	if(!strcmp(path, "/dev/null")) {
		return;
	} else if(!strcmp(path, "/dev/zero")) {
		for(int i = 0; i < maxLength; i++) {
			*(dest+i) = 0;
		}
	} else if(!strcmp(path, "/dev/example")) {
		const char* exstr = R"===(
{
    "version":[1,0,0],
    "name":"Chocolate Bananna",
    "category":"snacks",
    "servings":1.8,
    "cooktime":2,
    "ingredients":[{
        "amount":3.6,
        "unit":"UNIT",
        "name":"chocolate"
    },{
        "amount":8.1,
        "unit":"UNIT",
        "name":"banannas"
    }],
    "steps":[{
        "number":0,
        "text":"eat the chocolate"
    },{
        "number":1,
        "text":"eat the bananna"
    }]
}
)===";
		if((int) strlen(exstr) < maxLength) {
			strcpy(dest, exstr);
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

