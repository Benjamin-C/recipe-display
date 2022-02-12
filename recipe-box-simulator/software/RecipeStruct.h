/*
 * RecipeStruct.h
 *
 *  Created on: Feb 12, 2022
 *      Author: benjamin
 */

#ifndef SOFTWARE_RECIPESTRUCT_H_
#define SOFTWARE_RECIPESTRUCT_H_

enum IngrediantUnit {
    PIECES,
    CUP,
    TABLESPOON,
    TEASPOON
    // Add more as needed
};

// Struct for the ingredinats in a recipe
struct RecipeIngrediant {
    float amount; // The amonut of the ingrediant as a float, not fraction
    IngrediantUnit unit; // The unit of the amount
    char* name; // The name of the ingrediant
};

// Structs for the step in the recipe
struct RecipeStep {
    int number; // Step number in case stuff gets moved around
    char* text; // The instruction text
};

// Version numbering format. Print as major.minor.revision
struct VersionNumber {
    int major; // Major version number
    int minor; // Minor version number
    int rev; // Revision number
};

// Main recipe storage object
struct RecipeObejct {
    VersionNumber version; // Recipe storage version
    char* name; // Recipe name
    char* category; // Recipe category
    float servings; // Number of servings
    int cookTime; // Cook time in mins
    RecipeIngrediant* ingrediants; // List of ingrediants
    RecipeStep steps; // List of steps
};



#endif /* SOFTWARE_RECIPESTRUCT_H_ */
