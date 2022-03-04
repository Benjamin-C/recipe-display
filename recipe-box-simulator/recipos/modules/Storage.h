/* storage.h
 *
 * Storage module for RecipOS
 * Uses the storage driver to handle the storage media
 *
 * Author: Benjamin Crall
 */

#ifndef RECIPOS_MODULES_STORAGE_H_
#define RECIPOS_MODULES_STORAGE_H_

#include <string>
#include "../drivers/storage/StorageBackend.h"

class Storage {
public:
	Storage();
	virtual ~Storage();

	StorageBackend* sb;
	// Reads the maxlength bytes of a file, starting start bytes in.
	// WARNING this will crash the OS if dest can not hold maxlen characters
//	void readFile(const char* path, char* dest, int start, int maxLength);
	// Reads an entire file into the string
	std::string readFile(std::string path);
	// This doesn't work in the simulator because windows, the list is hard-coded
	FileList getDirectoryList(std::string path);
};

#endif /* RECIPOS_MODULES_STORAGE_H_ */
