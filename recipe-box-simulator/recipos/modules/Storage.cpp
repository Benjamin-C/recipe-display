/*
 * Storage.cpp
 *
 *  Created on: Jan 11, 2022
 *      Author: benjamin
 */

#include "Storage.h"
#include<string>

Storage::Storage() {
	sb = NULL;
}

Storage::~Storage() {
	// TODO Auto-generated destructor stub
}

std::string Storage::readFile(std::string path) {
	if(sb != NULL) {
		return sb->readFile(path);
	} else {
		return "Error - SB was null";
	}
}
// This doesn't work in the simulator because windows, the list is hard-coded
FileList Storage::getDirectoryList(std::string path) {
	if(sb != NULL) {
		return sb->getDirectoryList(path);
	} else {
		return {NULL, 0}; // Returns a file list with length 0
	}
}


