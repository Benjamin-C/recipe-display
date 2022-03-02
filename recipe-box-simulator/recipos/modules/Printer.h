/*
 * printer.h
 *
 *  Created on: Jan 11, 2022
 *      Author: benjamin
 */

#ifndef RECIPOS_MODULES_PRINTER_H_
#define RECIPOS_MODULES_PRINTER_H_

#include "recipe-box-simulator\software\RecipeUtils.h"
#include <iostream>
#include <string>
#include <vector>
//#include "lib\Adafruit_Thermal.h"                     // will deal with these later
//#include "lib\SoftwareSerial.h"

class Printer {
public:
	Printer(void);
    Printer(int RxPin, int TxPin);
	~Printer();
    int printText(char* textToPrint, int len);
    int printText(std::string strToPrint);
	int printRecipe(const Recipe* recipeToPrint);

private:
    void printTest();                                   // <-DELETEME: for debug only
    void sendPrint();
    int queueHeader(const Recipe* recipeToPrint);
    int queueIngredients(const Recipe* recipeToPrint);
    int queueBody(const Recipe* recipeToPrint);
    std::string tokenizeText(int& lineStartPos, const int tokenMaxW, const std::string src);
    std::vector<std::string> printQueue;
    //SoftwareSerial* SWSerial;                         // will deal with these later
    //Adafruit_Thermal* HW;
};

#endif /* RECIPOS_MODULES_PRINTER_H_ */
