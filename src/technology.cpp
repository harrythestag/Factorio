
#include "technology.h"
#include "item.h"
#include "recipe.h"
#include "event.h"
#include <iostream>

using namespace std;

extern unordered_map<string, Technology> technologies;
extern unordered_map<string, Item> items;
extern unordered_map<string, Recipe> recipes;
extern int indentation;

void Technology::research() {
	indentation++;
	cout << string(indentation * 2, ' ') << "researching technology: " << name << endl;
	// fulfill prerequisites
	bool prerequisit_outstanding = false;
	for (auto& pre : prerequisites) {
		if (!technologies[pre].researched) {
			technologies[pre].research();
			prerequisit_outstanding = true;
		}
	}
	if (prerequisit_outstanding) {
		indentation--;
		return;
	}

	// accumulate ingredients
	bool target_added = false;
	for (auto& ingredient : ingredients) {
		if (items[ingredient.first].amount < ingredient.second) { // note: if-condition may be redundant because it is checked by Item::craft
			items[ingredient.first].add_to_targets(ingredient.second);
			target_added = true;
		}
	}
	if (target_added) {
		indentation--;
		return;
	}

	create_research_event(name);

	// remove ingredients
	for (auto& ingredient : ingredients) {
		items[ingredient.first].amount -= ingredient.second;
	}

	// unlock recipes
	for (auto& recipe : recipes_unlocked) {
		recipes[recipe].enabled = true;
	}
	researched = true;
	indentation--;
}
