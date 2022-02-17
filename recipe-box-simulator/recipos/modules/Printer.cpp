/*
 * printer.cpp
 *
 *  Created on: Feb 9, 2022
 *      Author: Kyle P.
 */

#include "Printer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#define DEF_PRINTERPIN_TX 27
#define DEF_PRINTERPIN_RX 15
#define PAPER_CHARWIDTH 32
#define PAPER_CHARWIDTH 32

Printer::Printer(void) {
    //this->SWSerial = new SoftwareSerial(DEF_PRINTERPIN_RX, DEF_PRINTERPIN_TX);
    //this->HW = new Adafruit_Thermal(&SWSerial);

    this->printTest();
}

Printer::Printer(int RxPin, int TxPin) {
    //this->SWSerial = new SoftwareSerial(RxPin, TxPin);
    //this->HW = new Adafruit_Thermal(this->SWSerial);
}

Printer::~Printer() {

}

int Printer::printText(char* textToPrint) {
    return 0;
}

int Printer::printRecipe(const Recipe* recipeToPrint) {         // format, queue recipe data
    this->printQueue.clear();
    int linesToPrint = 0;

    linesToPrint += this->queueHeader(recipeToPrint);
    linesToPrint += this->queueIngredients(recipeToPrint);
    linesToPrint += this->queueBody(recipeToPrint);

    this->sendPrint();
    return linesToPrint;
}

int Printer::queueHeader(const Recipe* recipeToPrint) {
    int lines = 0;                                                                                                      // line counter
    std::string whitespace = " \n\r\t\f\v";                                                                             // "bank" of whitespace characters
    std::string lineOut;                                                                                                // line output workspace

    this->printQueue.push_back(" ");                                                                                    // queue blank line, increment counter
    lines++;

    std::string titleStr = recipeToPrint->name;                                                                         // assemble, push recipe title
    if (titleStr.length() == 0 || (titleStr.find_first_not_of(whitespace)) == (titleStr.length()-1)) {
        lineOut = "UNTITLED RECIPE";                                                                                    // if invalid name, use placeholder
        lineOut.insert(0,(PAPER_CHARWIDTH-lineOut.length())/2,' ');
        this->printQueue.push_back(lineOut);
        lines++;
    } else {                                                                                                            // otherwise, tokenize, pad, and push:
        int strStartPos = 0;
        lineOut = tokenizeText(strStartPos, PAPER_CHARWIDTH, titleStr);
        while (lineOut.length() > 0) {                                                                                  // print title in paper-width chunks until done
            lineOut.insert(0,(PAPER_CHARWIDTH-lineOut.length())/2,' ');
            this->printQueue.push_back(lineOut);
            lines++;
            lineOut = tokenizeText(strStartPos, PAPER_CHARWIDTH, titleStr);
        }
    }

    std::string categoryStr = recipeToPrint->category;
    lineOut = "(" + categoryStr + ": "  + std::to_string(recipeToPrint->cookTime) + " minutes)";            // assemble, pad, and push subtitle:
    lineOut.insert(0,(PAPER_CHARWIDTH-lineOut.length())/2,' ');
    this->printQueue.push_back(lineOut);
    lines++;

    return lines;
}

int Printer::queueIngredients(const Recipe* recipeToPrint) {
    int lines = 0;                                                                                                      // line counter
    std::string whitespace = " \n\r\t\f\v";                                                                             // "bank" of whitespace characters
    std::string lineOut;                                                                                                // line output workspace
    std::string unit;

    std::vector<RecipeIngredient> rIngredients;                                                                         // build steps into vector for easy manipulation
    if (recipeToPrint->ingredientCount > 0) {
        for (int i=0; i<(recipeToPrint->ingredientCount); i++) {
            rIngredients.push_back(recipeToPrint->ingredients[i]);
        }
    } else {
        return 0;
    }

    this->printQueue.push_back(" ");                                                                                    // queue blank line, increment counter
    lines++;
    this->printQueue.push_back("INGREDIENTS:");                                                                          // queue ingredient list header, increment counter
    lines++;

    for (RecipeIngredient rI: rIngredients) {
        lineOut = "* ";
        lineOut.append(std::to_string((int)(rI.amount)));
        lineOut.append(" ");
        unit.erase();
        switch (rI.unit) {
            case PIECES:        unit = "piece"; break;
            case CUP:           unit = "cup"; break;
            case TABLESPOON:    unit = "tablespoon"; break;
            case TEASPOON:      unit = "teaspoon"; break;
            default:            unit = "unit"; break;
        }
        lineOut.append(unit + " ");
        lineOut.append(rI.name);                                                                                        // TODO: needs to be tokenizeable
        this->printQueue.push_back(lineOut);
        lines++;
    }

    return lines;
}

int Printer::queueBody(const Recipe* recipeToPrint) {
    int lines = 0;                                                                                                      // line counter
    std::string whitespace = " \n\r\t\f\v";                                                                             // "bank" of whitespace characters
    std::string lineOut;                                                                                                // line output workspace

    std::vector<RecipeStep> rSteps;                                                                                     // build steps into vector for easy manipulation
    if (recipeToPrint->stepCount > 0) {
        for (int i=0; i<(recipeToPrint->stepCount); i++) {
            rSteps.push_back(recipeToPrint->steps[i]);
        }
    } else {
        return 0;
    }

    for (RecipeStep rS: rSteps) {
        this->printQueue.push_back(" ");                                                                                // queue blank line, increment counter
        lines++;
        this->printQueue.push_back("STEP " + std::to_string(rS.number) + ".");
        lines++;
        int strStartPos = 0;
        lineOut = tokenizeText(strStartPos, PAPER_CHARWIDTH, rS.text);                                            // TODO: figure out whitespace linebreak irregularities
        while (lineOut.length() > 0) {                                                                                  // print step in paper-width chunks until done
            this->printQueue.push_back(lineOut);
            lines++;
            lineOut = tokenizeText(strStartPos, PAPER_CHARWIDTH, rS.text);
        }
    }

    this->printQueue.push_back(" ");                                                                                    // queue blank line, increment counter
    lines++;
    return lines;
}

std::string Printer::tokenizeText(int& lineStartPos, const int tokenMaxW, const std::string src) {
    std::string whitespace = " \n\r\t\f\v";                                                                             // "bank" of whitespace chars
    lineStartPos = src.find_first_not_of(whitespace,lineStartPos);                                                      // skip over any leading whitespace
    if (lineStartPos == src.length()-1) {
        return "";                                                                                                      // if startPos==end of string, return empty
    }
    std::string outLine;
                                                                                                                        // find end position:
    int lineEndPos = src.find_last_not_of(whitespace, lineStartPos + std::min((int)(tokenMaxW), (int)(src.length()-lineStartPos)));
    if (lineEndPos == lineStartPos+tokenMaxW) {                                                                         // if max W reached (no whitespace):
        outLine = src.substr(lineStartPos, tokenMaxW-1).append("-");                                               // decrement width, append hyphen; return
        lineStartPos += tokenMaxW-1;
        return outLine;
    }

    outLine = src.substr(lineStartPos, lineEndPos-lineStartPos+1);                                                   // copy target src substring
    lineStartPos = std::min((int)lineEndPos+1,(int)src.length()-1);                                                     // set new start position
    return outLine;                                                                                                     // return substring
}

void Printer::printTest() {                                     // DEBUG for testing DELETEME
    Recipe* testRecipe = new Recipe();
    testRecipe->name = new char[128];
    strcpy(testRecipe->name, "CHIMKIN NOODLS");
    testRecipe->category = new char[128];
    strcpy(testRecipe->category, "SOOP");
    testRecipe->cookTime = 5;
    testRecipe->ingredientCount = 2;
    testRecipe->ingredients = new RecipeIngredient[2] {{1, PIECES, new char[128]},{3, CUP, new char[128]}};
    strcpy(testRecipe->ingredients[0].name, "CHIMKIN");
    strcpy(testRecipe->ingredients[1].name, "NOODLS");
    testRecipe->steps = new RecipeStep[3];
    testRecipe->steps[0].number = 1;
    testRecipe->steps[0].text = new char[128];
    strcpy(testRecipe->steps[0].text, "make soop! make soop!! make soop! make soop!!! make soop!! make soop! make soop!!!");
    testRecipe->steps[1].number = 2;
    testRecipe->steps[1].text = new char[128];
    strcpy(testRecipe->steps[1].text, "eat soop!! eat soop!!! eat soop! eat soop!! eat soop!! eat soop! eat soop!!! eat soop!!");
    testRecipe->steps[2].number = 3;
    testRecipe->steps[2].text = new char[128];
    strcpy(testRecipe->steps[2].text, "enjoy soop.");
    testRecipe->stepCount = 3;
    std::cout << "\n***BEGIN PRINTER TEST***\n" << std::endl;
    this->printRecipe(testRecipe);
    std::cout << "\n***END PRINTER TEST***\n" << std::endl;
}

void Printer::sendPrint() {                                     // write queue to printFile
    std::string border = "";
    border.append(PAPER_CHARWIDTH+6,'-');
    std::cout << "\n" << border << std::endl;
    int count = 0;
    for (std::string s:printQueue) {
        s.append(PAPER_CHARWIDTH - s.length(), ' ');
        std::cout << "|  " << s << "  | " << count++ << std::endl;
    }
    std::cout << border << "\n" << std::endl;
}