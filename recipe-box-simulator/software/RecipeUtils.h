/*
 * RecipeStruct.h
 *
 *  Created on: Feb 12, 2022
 *      Author: benjamin
 */

#ifndef SOFTWARE_RECIPEUTILS_H_
#define SOFTWARE_RECIPEUTILS_H_

enum IngredientUnit {
    PIECES,
    CUP,
    TABLESPOON,
    TEASPOON
    // Add more as needed
};

// Struct for the ingredinats in a recipe
struct RecipeIngredient {
    float amount; // The amonut of the ingrediant as a float, not fraction
    IngredientUnit unit; // The unit of the amount
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
struct Recipe {
    VersionNumber version; // Recipe storage version
    char* name; // Recipe name
    char* category; // Recipe category
    float servings; // Number of servings
    int cookTime; // Cook time in mins
    int ingredientCount; // Number of ingredient
    RecipeIngredient* ingredients; // List of ingredients
    int stepCount; // Number of steps
    RecipeStep* steps; // List of steps
};

// Gets the example RecipeObject
Recipe* getExampleRO(void);
// Prints a RecipeObject
void printRecipeObject(Recipe* ro);
// Prints a unit, and adds 's' if amount != 0
void printUnit(IngredientUnit iu, float amount);
// Dynamically allocates a string;
char* makeString(const char* string);
// There should probably be a destroy method here. There is not. Too bad.

#endif /* SOFTWARE_RECIPEUTILS_H_ */
