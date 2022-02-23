/*
 * RecipeStruct.h
 *
 *  Created on: Feb 12, 2022
 *      Author: benjamin
 */

#ifndef SOFTWARE_RECIPEUTILS_H_
#define SOFTWARE_RECIPEUTILS_H_

#include <string>

#define MAX_RECIPE_SIZE 256

namespace RecipeUtils {
// Update this if longer ingredient names are used
// Also update the parser and the printer
#define MAX_INGREDIENT_UNIT_LENGTH 16
	enum IngredientUnit {
		PIECE,
		CUP,
		TABLESPOON,
		TEASPOON,
		NOTHING // It's null, as you would expect
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
	Recipe* getExampleRecipe(void);
	// Prints a RecipeObject
	void printRecipe(Recipe* re);
	// Prints a unit, and adds 's' if amount != 0
	void printUnit(IngredientUnit iu, float amount);
	void printVersion(VersionNumber v);

	// There should probably be a destroy method here. There is not. Too bad.
	void destroyRecipe(Recipe* r);

	Recipe* parseWithOptions(std::string stdstr, bool includeIngredients, bool includeSteps);
	Recipe* parseString(std::string stdstr);

	int countJSONArray(const char* read);


	RecipeIngredient parseIngredient(const char** readhead);
	RecipeStep parseStep(const char** readhead);
	const char* maybeStoreQuotedString(const char* test, const char* readhead, std::string* dest);
	const char* maybeStoreFloat(const char* test, const char* readhead, float* dest);
	const char* maybeStoreInt(const char* test, const char* readhead, int* dest);

	int readInt(const char** str);
	float readFloat(const char** str);
	const char* quoteComp(const char* str, const char* test);
	int quoteLen(const char* str);
	void quoteCpy(char* dest, const char** str);

	IngredientUnit parseUnit(std::string str);
}


#endif /* SOFTWARE_RECIPEUTILS_H_ */
