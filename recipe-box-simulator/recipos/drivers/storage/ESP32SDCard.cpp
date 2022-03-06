/*
 * ESP32SDCard.cpp
 *
 *  Created on: Mar 3, 2022
 *      Author: benjamin
 */

#include "ESP32SDCard.h"

#include <Arduino.h>

#include <SD.h>
#include <SPI.h>

#include "../../RecipOS.h"

// please fix errors
int ESP32SDCard::countFiles(std::string path) {
#ifdef ESP32
  File dir = SD.open(path.c_str()); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  int count = 0;
  while(true) {
    File entry = dir.openNextFile(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
    if(!entry) {
      return count;
    } else {
      if(!entry.isDirectory()) { // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
        count++;
      }
    }
    entry.close(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  }
  dir.close(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
#endif
}

FileList ESP32SDCard::getDirectoryList(std::string path) {
#ifdef ESP32
  int count = countFiles(path);
  FileList fl = { NULL, 0 };
  fl.list = new std::string[count];
  fl.count = count;
  File dir = SD.open("/"); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  dir.rewindDirectory(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  for(int i = 0; i < count; i++) {
    File entry =  dir.openNextFile(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
    if(!entry) {
      break;
    }
    if(!entry.isDirectory()) { // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
      fl.list[i] = entry.name(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
    } else {
      i--; // Don't count this one
    }
    entry.close(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  }
  dir.close(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  return fl;
#endif
}

std::string ESP32SDCard::readFile(std::string path) {
#ifdef ESP32
  File f = SD.open(path.c_str()); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  if(f) {
    int s = f.size(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
    char* str = new char[s+1]; // Add one to the size to make sure there is room for a null;
    char* w = str;
    for(int i = 0; i < s; i++) {
      *w++ = f.read(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
    }
    *w = '\0';
    f.close(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
    return str;
  }
  f.close(); // @suppress("Type cannot be resolved") // @suppress("Method cannot be resolved")
  return "Error!";
#endif
}

ESP32SDCard::ESP32SDCard() {
	// TODO Auto-generated constructor stub
	Serial.print("Initializing SD card...");

	if (!SD.begin(32)) {
		Serial.println("initialization failed!");
		FIX_STD_STRINGS();
		makeBSOD("SD card initialization failed.Please make sure that the cardis inserted and that there    are no loose connections.");
		return;
	}
	Serial.println("initialization done.");

}

ESP32SDCard::~ESP32SDCard() {
	// TODO Auto-generated destructor stub
}

