#ifndef STORAGE_BACKEND_H
#define STORAGE_BACKEND_H

#include <string>

struct FileList {
	std::string* list;
	int count;
};

class StorageBackend {
public:
	virtual ~StorageBackend() { };
	// Reads the maxlength bytes of a file, starting start bytes in.
	// WARNING this will crash the OS if dest can not hold maxlen characters
//	void readFile(const char* path, char* dest, int start, int maxLength);
	// Reads an entire file into the string
	virtual std::string readFile(std::string path) = 0;
	// This doesn't work in the simulator because windows, the list is hard-coded
	virtual FileList getDirectoryList(std::string path) = 0;
};

#endif
