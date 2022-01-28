
#include "recipe.h"
#include "technology.h"
#include "factory.h"
#include "item.h"
#include "event.h"
#include <iostream>

using namespace std;

extern unordered_map<string, Technology> technologies;
extern unordered_map<string, Factory> factories;
extern vector<Factory> current_factories;
extern unordered_map<string, Item> items;
extern int indentation;

void Recipe::execute(int num) {
	indentation++;
	cout << string(indentation * 2, ' ') << "executing recipe: " << name << endl;

	if (!enabled) {
		cout << string(indentation * 2, ' ') << "but first we have to research the recipe" << endl;
		technologies[enabled_by].research();
		indentation--;
		return;
		cout << string(indentation * 2, ' ') << "continue executing recipe: " << name << endl;
	}

	bool factory_available = false;
	int i;
	for (i = 0; i < current_factories.size(); ++i) {
		Factory fac = current_factories[i];
		if (fac.categories[category]) {
			factory_available = true;
			break;
		}
	}
	if (!factory_available) {
		cout << string(indentation * 2, ' ') << "but first we have to build a factory" << endl;
		for (auto& elem : factories) {
			Factory fac = elem.second;
			if (fac.categories[category]) {
				if (items[fac.name].amount == 0) {
					fac.craft();
					indentation--;
					return;
				} else {
					Factory::build(fac);
				}
				break;
			}
		}
	}

	//just a little bit of math so we dont overproduce insane amounts
	int num_executions = ceil(float(num) / products[0].second);
	// accumulate ingredients
	bool target_added = false;
	for (auto& ingredient : ingredients) {
		int ingredients_needed = ingredient.second * num_executions;
		// put ingredients, that are not zet available on the target stack
		if (items[ingredient.first].amount < ingredients_needed) {
			items[ingredient.first].add_to_targets(ingredients_needed);
			target_added = true;
		}
	}
	if (target_added) {
		indentation--;
		return;
	}

	// start factory
	create_start_factory_event(current_factories[i].id, name);

	// timestamp += energy
	Event::timestamp += energy * (num_executions); // because apparently i dont have enough chemical science packs

	// stop factory
	create_stop_factory_event(current_factories[i].id);

	// remove ingredients
	for (auto& ingredient : ingredients) {
		items[ingredient.first].amount -= ingredient.second * num_executions;
	}

	// add products
	for (auto& prod : products) {
		items[prod.first].amount += prod.second * num_executions;
	}
	indentation--;
}
