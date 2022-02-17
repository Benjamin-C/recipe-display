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

	ro->name = "Chocolate Bananana";
	ro->category = "Snacks";
	ro->servings = 1.5;
	ro->cookTime = 4;

	ri[0].amount = 1.5;
	ri[0].unit = PIECES;
	ri[0].name = "Bananana";

	ri[1].amount = 0.76;
	ri[1].unit = CUP;
	ri[1].name = "Chocolate";

	ro->ingredients = ri;
	ro->ingredientCount = 2;

	rs[0].number = 1;
	rs[0].text = "Eat bananna";

	rs[1].number = 2;
	rs[1].text = "Eat chocolate";

	ro->steps = rs;
	ro->stepCount = 2;

	return ro;
}

void RecipeUtils::printRecipe(Recipe* ro) {
	printf("--- Recipe Object ---\n");
	printf("  Version: "); printVersion(ro->version); printf("\n");
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

void RecipeUtils::destroyRecipe(Recipe* ro) {

	free(ro->ingredients);

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

	// Find version number
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

	// Only continue if this version is supported
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
				char* test = quoteComp(read, "ingredients");
				if(test != NULL) {
					read = test;
					while(*read != '\0' && *read != ']') {
						RecipeUtils::RecipeIngredient* i = RecipeUtils::parseIngredient(&read);
						read++;
					}
					// Should read ingredients here
					while(*read++ != ']') { }
				}

				test = quoteComp(read, "steps");
				if(test != NULL) {
					// Should read ingredients here
					while(*read++ != ']') { }
					break;
				}

				// Steps
//				printf("\nno\n");
			}
			read++;
		}

		return r;
	} else {
		printf("Invalid recipe version");
		return NULL;
	}
}

RecipeUtils::RecipeIngredient* RecipeUtils::parseIngredient(char** readhead) {
	char* read = *(readhead);

	RecipeUtils::RecipeIngredient* i = (RecipeUtils::RecipeIngredient*) malloc(sizeof(RecipeUtils::RecipeIngredient));
	while(*read++ != '{') { };
	if(i != NULL) {
		while(*read != '\0' && *read != '}') {
			if(*read == '"') {
				read = maybeStoreFloat("amount", read, &(i->amount));
				read = maybeStoreQuotedString("name", read, &(i->name));
				// Still need to parse units
			}
			read++;
		}
		while(*read != '}') {
			read++;
		};
	} else {
		printf("Could not allocate RecipeIngrediant");
	}

	*readhead = read;
	return i;
}

char* RecipeUtils::maybeStoreQuotedString(const char* test, char* readhead, std::string* dest) {
	char* next = quoteComp(readhead, test);
	if(next != NULL) {
		next += 2;
		char* name = (char*) malloc(sizeof(char) * (quoteLen(next) + 1));
		if(name != NULL) {
			next++;
			quoteCpy(name, &next);
			dest->assign(name);
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
			*(*str)++;
		}
		n += d / (float) (10.0*i);
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
	return (*str == '"' && *test == '\0') ? str : NULL;

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
