/* storage.h
 *
 * Storage module for RecipOS
 * Uses the storage driver to handle the storage media
 *
 * Author: Benjamin Crall
 */

#ifndef RECIPOS_MODULES_STORAGE_H_
#define RECIPOS_MODULES_STORAGE_H_

class Storage {
public:
	Storage();
	virtual ~Storage();
	// Reads the maxlength bytes of a file, starting start bytes in.
	// WARNING this will crash the OS if dest can not hold maxlen characters
	void readFile(char* path, char* dest, int start, int maxLength);
};

#endif /* RECIPOS_MODULES_STORAGE_H_ */
