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
	ri[0].unit = PIECE;
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
	case PIECE: printf("piece"); break;
	case CUP: printf("cup"); break;
	case TABLESPOON: printf("tablespoon"); break;
	case TEASPOON: printf("teaspoon"); break;
	case NOTHING: printf("nothing"); return; // Don't print s for plural
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
				char* igtTest = quoteComp(read, "ingredients");
				if(igtTest != NULL) {
					read = igtTest;
					int igtNum = countJSONArray(read);
					// Count ingredi
					RecipeIngredient* ria = new RecipeIngredient[igtNum];
					int igtPos = 0;
					if(ria != NULL) {
						while(*read != '\0' && *read != ']' && igtPos < igtNum) {
							RecipeIngredient igt = parseIngredient(&read);
							if(igt.amount > 0) {
								ria[igtPos++] = igt;
							}
							read++;
						}
						r->ingredientCount = igtNum;
						r->ingredients = ria;
					} else {
						printf("Ingredient array allocation failed");
					}
					// Should read ingredients here
					while(*read++ != ']') { }
				}

				char* stpTest = quoteComp(read, "steps");
				if(stpTest != NULL) {
					read = stpTest;
					int stpNum = countJSONArray(read);
					// Count ingredi
					RecipeStep* sta = new RecipeStep[stpNum];
					int stpPos = 0;
					if(sta != NULL) {
						while(*read != '\0' && *read != ']' && stpPos < stpNum) {
							RecipeStep igt = parseStep(&read);
							if(igt.number >= 0) {
								sta[stpPos++] = igt;
							}
							read++;
						}
						r->stepCount = stpNum;
						r->steps = sta;
					} else {
						printf("Ingredient array allocation failed");
					}
					// Should read ingredients here
					while(*read++ != ']') { }
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

int RecipeUtils::countJSONArray(char* read) {
	// Find beginning of array
	while(*read++ != '[') { }
	int count = 0;
	while(*read != ']') {
		// Find first opening bracket
		while(*read++ != '{') {
			if(*read == ']') {
				return -1;
			}
		};
		// Find closing bracket
		while(*read++ != '}') {
			if(*read == ']') {
				return -1;
			}
		};
		count++;
	}
	// Return -1 if the curly brackets were still open at the end
	return count;
}

RecipeUtils::RecipeIngredient RecipeUtils::parseIngredient(char** readhead) {
	char* read = *(readhead);

	RecipeUtils::RecipeIngredient i;
	i.amount = 0;
	while(*read++ != '{') { };

	while(*read != '\0' && *read != '}') {
		if(*read == '"') {
			read = maybeStoreFloat("amount", read, &(i.amount));
			read = maybeStoreQuotedString("name", read, &(i.name));
			std::string unitstr = "";
			read = maybeStoreQuotedString("unit", read, &unitstr);
			if(unitstr != "") {
				i.unit = parseUnit(unitstr);
			}

		}
		read++;
	}
	while(*read != '}') {
		read++;
	};

	*readhead = read;
	return i;
}

RecipeUtils::RecipeStep RecipeUtils::parseStep(char** readhead) {
	char* read = *(readhead);

	RecipeUtils::RecipeStep s;
	s.number = -1;
	while(*read++ != '{') { };

	while(*read != '\0' && *read != '}') {
		if(*read == '"') {
			read = maybeStoreInt("amount", read, &(s.number));
			read = maybeStoreQuotedString("name", read, &(s.text));
		}
		read++;
	}
	while(*read != '}') {
		read++;
	};

	*readhead = read;
	return s;
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

RecipeUtils::IngredientUnit RecipeUtils::parseUnit(std::string str) {
	if(str == "PIECE" || str == "UNIT") {
		return RecipeUtils::PIECE;
	} else if(str == "CUP") {
		return RecipeUtils::CUP;
	} else if(str == "TABLESPOON") {
		return RecipeUtils::TABLESPOON;
	} else if(str == "TEASPOON") {
		return RecipeUtils::TEASPOON;
	}
	return RecipeUtils::NOTHING;
}
