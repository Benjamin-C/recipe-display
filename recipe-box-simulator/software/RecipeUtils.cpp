#include "RecipeUtils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

RecipeUtils::Recipe* RecipeUtils::getExampleRecipe(void) {
	Recipe* ro = new RecipeUtils::Recipe;

	RecipeIngredient* ri = (RecipeIngredient*) malloc(sizeof(RecipeIngredient)*2);
	RecipeStep* rs = (RecipeStep*)malloc(sizeof(RecipeStep)*2);

	if(ri == NULL || rs == NULL) {
		printf("Allocation error\n");
		return NULL;
	}

	ro->version.major = 1;
	ro->version.minor = 0;
	ro->version.rev = 0;

	ro->name = "Chocolate Bananana";
	ro->category = "Snacks";
	ro->servings = 1.5;
	ro->cookTime = 4;

	ri[0].amount = 1.5;
	ri[0].unit = RecipeUtils::PIECES;
	ri[0].name = "Bananana";

	ri[1].amount = 0.76;
	ri[1].unit = RecipeUtils::CUP;
	ri[1].name = "Chocolate";

	ro->ingredients = ri;
	ro->ingredientCount = 2;

	rs[0].number = 1;
	rs[0].text = "Eat bananana";

	rs[1].number = 2;
	rs[1].text = "Eat chocolate";

	ro->steps = rs;
	ro->stepCount = 2;

	return ro;
}

void RecipeUtils::printRecipe(Recipe* ro) {
	printf("--- Recipe Object ---\n");
	printf("  Version: %d.%d.%d\n", ro->version.major, ro->version.minor, ro->version.rev);
	printf("  Name: %s\n", ro->name.c_str());
	printf("  Category: %s\n", ro->category.c_str());
	printf("  Servings: %.1f\n", ro->servings);
	printf("  Cook time: %dmin\n", ro->cookTime);
	printf("  Ingredients:\n");
	for(int i = 0; i < ro->ingredientCount; i++) {
		printf("    %.2f ", ro->ingredients[i].amount);
		printUnit(ro->ingredients[i].unit, ro->ingredients[i].amount);
		printf(" %s\n", ro->ingredients[i].name.c_str());
	}

	printf("  Steps: (%d)\n", ro->stepCount);
	for(int s = 0; s < ro->stepCount; s++) {
		printf("    %d: %s\n", ro->steps[s].number, ro->steps[s].text.c_str());
	}
	printf("---------------------\n");
}

void RecipeUtils::printVersion(VersionNumber v) {
	printf("%d.%d.%d", v.major, v.minor, v.rev);
}

void RecipeUtils::printUnit(IngredientUnit iu, float amount) {
	switch(iu) {
	case PIECES: printf("piece"); break;
	case CUP: printf("cup"); break;
	case TABLESPOON: printf("tablespoon"); break;
	case TEASPOON: printf("teaspoon"); break;
	}
	if(amount != 1) {
		printf("s");
	}
}
