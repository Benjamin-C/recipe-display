/*
 * ComputerFiles.h
 *
 *  Created on: Mar 3, 2022
 *      Author: benjamin
 */

#ifndef RECIPE_BOX_SIMULATOR_RECIPOS_DRIVERS_STORAGE_COMPUTERFILES_H_
#define RECIPE_BOX_SIMULATOR_RECIPOS_DRIVERS_STORAGE_COMPUTERFILES_H_

#include "StorageBackend.h"

#ifdef ESP32
#warning "The ComputerFiles storage backend will not work on ESP32"
#endif

class ComputerFiles {
public:
	ComputerFiles();
	virtual ~ComputerFiles();

	// Reads an entire file into the string
	std::string readFile(std::string path);
	// This doesn't work in the simulator because windows, the list is hard-coded
	FileList getDirectoryList(std::string path);
private:
	int countFiles(std::string path);
};

#endif /* RECIPE_BOX_SIMULATOR_RECIPOS_DRIVERS_STORAGE_COMPUTERFILES_H_ */
