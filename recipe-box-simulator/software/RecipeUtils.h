/*
 * RecipeStruct.h
 *
 *  Created on: Feb 12, 2022
 *      Author: benjamin
 */

#ifndef SOFTWARE_RECIPEUTILS_H_
#define SOFTWARE_RECIPEUTILS_H_

#include <string>

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
    std::string name; // The name of the ingrediant
};

// Structs for the step in the recipe
struct RecipeStep {
    int number; // Step number in case stuff gets moved around
    std::string text; // The instruction text
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
    std::string name; // Recipe name
    std::string category; // Recipe category
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

// There should probably be a destroy method here. There is not. Too bad.

#endif /* SOFTWARE_RECIPEUTILS_H_ */
