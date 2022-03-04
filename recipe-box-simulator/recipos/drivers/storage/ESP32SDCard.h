/*
 * ESP32SDCard.h
 *
 *  Created on: Mar 3, 2022
 *      Author: benjamin
 */

#ifndef RECIPE_BOX_SIMULATOR_RECIPOS_DRIVERS_STORAGE_ESP32SDCARD_H_
#define RECIPE_BOX_SIMULATOR_RECIPOS_DRIVERS_STORAGE_ESP32SDCARD_H_

#include "StorageBackend.h"

class ESP32SDCard : public StorageBackend {
public:
	ESP32SDCard();
	virtual ~ESP32SDCard();

	// Reads an entire file into the string
	std::string readFile(std::string path);
	// This doesn't work in the simulator because windows, the list is hard-coded
	FileList getDirectoryList(std::string path);
private:
	int countFiles(std::string path);
};

#endif /* RECIPE_BOX_SIMULATOR_RECIPOS_DRIVERS_STORAGE_ESP32SDCARD_H_ */
