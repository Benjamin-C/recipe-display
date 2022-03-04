/*
 * ComputerFiles.cpp
 *
 *  Created on: Mar 3, 2022
 *      Author: benjamin
 */

#include "ComputerFiles.h"

#ifndef ESP32
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<filesystem>
#endif

ComputerFiles::ComputerFiles() {
	// TODO Auto-generated constructor stub

}

ComputerFiles::~ComputerFiles() {
	// TODO Auto-generated destructor stub
}

std::string ComputerFiles::readFile(std::string path) {
#ifndef ESP32
//void Storage::readFile(const char* path, char* dest, int start, int maxLength) {
	if(path == "/dev/null") {
		return "";
	} else if(path == "/dev/zero") {
//		for(int i = 0; i < maxLength; i++) {
//			*(dest+i) = 0;
//		}
		return "0";
	} else if(path == "/dev/example") {
		return R"===(
{
    "version":[1,0,0],
    "name":"Chocolate Bananana",
    "category":"snacks",
    "servings":1.8,
    "cooktime":2,
    "ingredients":[{
        "amount":3.6,
        "unit":"UNIT",
        "name":"chocolate"
    },{
        "amount":8.1,
        "unit":"CUP",
        "name":"Bananana"
    }],
    "steps":[{
        "number":0,
        "text":"eat the chocolate"
    },{
        "number":1,
        "text":"eat the bananana"
    }]
}
)===";
	} else {
		using namespace std;
		ifstream f("./storage/" + path); //taking file as inputstream
		string str;
		if(f) {
			ostringstream ss;
			ss << f.rdbuf(); // reading data
			str = ss.str();
			return str;
		} else {
			printf("Something wrong, probably file doesn't exist");
			return "404";
		}
	}
#endif
}

FileList ComputerFiles::getDirectoryList(std::string path) {
#ifndef ESP32
	FileList fl;
	fl.list = new std::string[8];
	fl.list[0] = "bbq_lasagne.json";
	fl.list[1] = "burnt_toast.json";
	fl.list[2] = "chocolate_cake.json";
	fl.list[3] = "deepdish_ravioli.json";
	fl.list[4] = "deep_fried_okra.json";
	fl.list[5] = "moist_butter.json";
	fl.list[6] = "potato_jam.json";
	fl.list[7] = "raspberry_filet.json";
	fl.count = 8;
	return fl;
#endif
}
