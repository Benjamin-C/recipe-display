Code Style Guide
=================
---
## Overview
1. Curly brackets share lines
2. Indent with tabs, not spaces
3. Plenty of comments
4. Condense code
5. Variables have descriptive names
6. File names are lower case unless they are for a class
---
## Details

### Curly brackets share lines
Put the opening curly brackets for blocks on the same line as the statement that opens the block

    // Good
    for(int i = 0; i < 7; i++) {
      printf("Hi %d\n", i);
    }

    // Bad
    for(int i = 0; i < 7; i++)
    {
      printf("Hi %d\n", i);
    }

### Indent with tabs, not spaces
Use a tab character to indent code. If this is not possible, use 4 spaces.

### Plenty of comments
Use plenty of comments in your code so that everyone knows what it does. Comments should be placed at a minimum
1. Before function declarations in headers stating what each argument does

        // Gets how many skittles of a specific color a player has
        int getSkittles(Color color);

2. Before function definitions in source files

        /* getSkittles()
         * Gets how many skittles a plyaer has.
         * May not work if the player is underwater
         * color - the Color of the skittles to count. Counts all colors if the color is black.
         */
        int Player::getSkittles(Color color) { ... }

3. At the top of every file stating the filename, what the file does, and the primary author(s) of the file

        /* Player.h
         *
         * Main code for the Player
         *
         * Author: Bob the Builder
         */

3. Anywhere else you think they are needed. This could be near the top of the header files describing data formats, general usage, or limitations.

### Condense code
Condense blocks of code any time it makes the overall code easier to read. This might be if there is only a single function call in a block. Also, use blank lines sparingly, but do use them where they improve readability, such as before function definitions or between chunks of related code. In general, keep code neat but readable.

    switch(color) {
    case GREEN: { printf("Grass is green!"); } break;
    case BLUE: { swim(); }
    case PINK: { startFire(color); }
    case RED: {
      for(int i = 0; i < numCows; i++) {
        if(i % 2 == 0) { cows[i].paint(RED); }
      }
    } break;
    default: {
      for(int i = 0; i < numCows; i++) {
        cows[i].paint(color);
      }
    } break;
    }
### Variables have descriptive names
* Variables should have useful names. They shouldn't be too long, (ex: theNumberOfPeopleWhoAitPie is too long)
* Variable names should be named in camel case with no underscores. (ex: skittleCount)
* Constant or #define names should be in all caps with underscores as needed to separate words (ex: MAX_SKITTLES)
* Single letter names are allowed as long as they have a common usage and/or are used in a small scope (ex: i,j,x,y,c are usually fine)
* Variable names must be unique in their scope. Thins includes different capitalization and punctuation, (ex: if you have numCows, you should not also have NumCows, num_cows, NUM_COWS, or _numCows).

### File names are lower case unless they are for a class
Files should be given detailed names, ending in .h for headers or .cpp for c++ source files. If the file is primarily for a c++ class, it should be named to match, in pascal case. If the file is not primarily for a class, it should be named in all lower case.
