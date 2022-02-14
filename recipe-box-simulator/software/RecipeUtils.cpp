#include "RecipeUtils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

RecipeUtils::Recipe* RecipeUtils::getExampleRecipe(void) {
	Recipe* ro = new Recipe;

	RecipeIngredient* ri = (RecipeIngredient*) malloc(sizeof(RecipeIngredient)*2);
	RecipeStep* rs = (RecipeStep*)malloc(sizeof(RecipeStep)*2);

	if(ri == NULL || rs == NULL) {
		printf("Allocation error\n");
		return NULL;
	}

	ro->version.major = 1;
	ro->version.minor = 0;
	ro->version.rev = 0;

	ro->name = makeString("Chocolate Bananna");
	ro->category = makeString("Snacks");
	ro->servings = 1.5;
	ro->cookTime = 4;

	ri[0].amount = 1.5;
	ri[0].unit = PIECES;
	ri[0].name = makeString("Bananna");

	ri[1].amount = 0.76;
	ri[1].unit = CUP;
	ri[1].name = makeString("Chocolate");

	ro->ingredients = ri;
	ro->ingredientCount = 2;

	rs[0].number = 1;
	rs[0].text = makeString("Eat bananna");

	rs[1].number = 2;
	rs[1].text = makeString("Eat chocolate");

	ro->steps = rs;
	ro->stepCount = 2;

	return ro;
}

void RecipeUtils::printRecipe(Recipe* ro) {
	printf("--- Recipe Object ---\n");
	printf("  Version: "); printVersion(ro->version); printf("\n");
	printf("  Name: %s\n", ro->name);
	printf("  Category: %s\n", ro->category);
	printf("  Servings: %.1f\n", ro->servings);
	printf("  Cook time: %dmin\n", ro->cookTime);
	printf("  Ingredients:\n");
	for(int i = 0; i < ro->ingredientCount; i++) {
		printf("    %.2f ", ro->ingredients[i].amount);
		printUnit(ro->ingredients[i].unit, ro->ingredients[i].amount);
		printf(" %s\n", ro->ingredients[i].name);
	}

	printf("  Steps: (%d)\n", ro->stepCount);
	for(int s = 0; s < ro->stepCount; s++) {
		printf("    %d: %s\n", ro->steps[s].number, ro->steps[s].text);
	}
	printf("---------------------\n");
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

void RecipeUtils::printVersion(VersionNumber v) {
	printf("%d.%d.%d", v.major, v.minor, v.rev);
}

char* RecipeUtils::makeString(const char* string) {
	char* s = (char*) malloc(sizeof(char) * strlen(string));
	if(s != NULL) {
		strcpy(s, string);
	} else {
		printf("AAAAAA String allocation failed (RecipeUtils.cpp)");
	}
	return s;
}

void RecipeUtils::destroyRecipe(Recipe* ro) {

	free(ro->name);
	free(ro->category);

	for(int i = 0; i < ro->ingredientCount; i++) {
		free(ro->ingredients[i].name);
	}
	free(ro->ingredients);

	for(int s = 0; s < ro->stepCount; s++) {
		free(ro->steps[s].text);
	}
	free(ro->steps);
}

RecipeUtils::Recipe* RecipeUtils::parseStrin(char* str) {
	// Only compatible with V1.0.0, currently does not check version
	Recipe* r = new Recipe;
	int len = strlen(str);

//	if(!(str[0] != '{' && str[len-1] == '}'))

	char* read = str;
	char* write = str;
	bool inQuotes = false;

	// Strip whitespace
	while(*read != '\0') {
		if(*read == '"') {
			inQuotes = !inQuotes;
			*write++ = *read;
		} else {
			if(inQuotes || !(*read == '\n' || *read == '\t' || *read == '\r' || *read == ' ')) {
				*write++ = *read;
			}
		}
		read++;
	}
	*write = '\0';

	printf("%s\n", str);

//	if(str[0] != '{' || str[len-1] != '}') {
//		printf("String was not JSON");
//		return NULL;
//	}

	// Split top level
	int squareCount = 0;
	inQuotes = false;
	read = str;

	printf("cowhide\n");

	char* verSearch = read;

	while(*verSearch != '\0') {
//		while(*read++ != '"') { }
		if(*verSearch == '"') {
			char* next = quoteComp(verSearch, "version");
			if(next != NULL) {
				next += 3;
				verSearch = next;
				r->version.major = readInt(&verSearch);
				verSearch++;
				r->version.minor = readInt(&verSearch);
				verSearch++;
				r->version.rev = readInt(&verSearch);
//				printf("Found version "); printVersion(r->version); printf("\n");
			}
		}
		verSearch++;
	}

	if(r->version.major == 1 && r->version.minor == 0 && r->version.rev == 0) {

		while(*read != '\0') {
	//		while(*read++ != '"') { }
			if(*read == '"') {
				// Recipe header info
				read = maybeStoreQuotedString("name", read, &(r->name));
				read = maybeStoreQuotedString("category", read, &(r->category));
				read = maybeStoreFloat("servings", read, &(r->servings));
				read = maybeStoreInt("cookTime", read, &(r->cookTime));
				// ingredient

				// Steps
				printf("\nno\n");
			}
			read++;
		}

		return r;
	} else {
		printf("Invalid recipe version");
		return NULL;
	}
}

char* RecipeUtils::maybeStoreQuotedString(const char* test, char* readhead, char** dest) {
	char* next = quoteComp(readhead, test);
	if(next != NULL) {
		next += 2;
		char* name = (char*) malloc(sizeof(char) * (quoteLen(next) + 1));
		if(name != NULL) {
			next++;
			quoteCpy(name, &next);
			*dest = name;
		} else {
			printf("malloc error! Fix it!");
		}
		return next;
	} else {
		return readhead;
	}
}

char* RecipeUtils::maybeStoreFloat(const char* test, char* readhead, float* dest) {
	char* next = quoteComp(readhead, test);
	if(next != NULL) {
		next += 2;
		*dest = readFloat(&next);
		return next;
	} else {
		return readhead;
	}
}

char* RecipeUtils::maybeStoreInt(const char* test, char* readhead, int* dest) {
	char* next = quoteComp(readhead, test);
	if(next != NULL) {
		next += 2;
		*dest = readInt(&next);
		return next;
	} else {
		return readhead;
	}

}

int RecipeUtils::readInt(char** str) {
	int n = 0;
	while(**str >= '0' && **str <= '9') {
		n *= 10;
		n += (**str) - '0';
		(*str)++;
	}
	return n;
}

float RecipeUtils::readFloat(char** str) {

	float n = 0;
	while(**str >= '0' && **str <= '9') {
		n *= 10;
		n += (**str) - '0';
		(*str)++;
	}
	if(**str == '.') {
		(*str)++;
		int i = 0;
		int d = 0;
		while(**str >= '0' && **str <= '9') {
			d += (**str) - '0';
			i++;
			char tmp = *(*str)++;
		}
		n += d / (10*i);
	}
	return n;
}

// Point str to the opening quotes
char* RecipeUtils::quoteComp(char* str, const char* test) {
	str++;
	while(*str != '\0' && *str != '"' && *test != '\0') {
//		printf("%c =? %c\n", *str, *test);
		if(*str++ != *test++) {
			return NULL;
		}
	}
	return (*str == '"') ? str : NULL;

}

int RecipeUtils::quoteLen(char* str) {
	int count = 0;
	str++;
	while(*str != '\0' && *str != '"') {
		str++;
		count++;
	}
	return count;
}

void RecipeUtils::quoteCpy(char* dest, char** str) {
	while(**str != '\0' && **str != '"') {
		*dest++ = **str;
		(*str)++;
	}
	*dest = '\0';
}
