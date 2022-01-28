#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <string>
#include "json.hpp"
#include "item.h"
#include "recipe.h"
#include "factory.h"
#include "technology.h"
#include "event.h"

using json = nlohmann::json;
using namespace std;

unordered_map<string, Item> items;
unordered_map<string, Item> goal_items;
vector<pair<string, int>> targets;
unordered_map<string, Recipe> recipes;
unordered_map<string, Factory> factories;
vector<Factory> current_factories;
unordered_map<string, Technology> technologies;
int indentation = 0;

void get_items() {
	std::ifstream item_fs("../data/factorio-data/item.json");
	json j;
	item_fs >> j;
	for (auto& elem : j.items()) {
		string k = elem.key();
		json v = elem.value();
		string vs = v["type"].get<string>();
		items[k] = Item(k, vs);
	}
	cout << "we have " << items.size() << " items" << endl;
}

void get_recipes() {
	std::ifstream recipe_fs("../data/factorio-data/recipe.json");
	json j;
	recipe_fs >> j;
	for (auto& elem : j.items()) {
		string k = elem.key();
		auto v = elem.value();
		Recipe r = v.get<Recipe>();
		r.name = k;
		recipes[k] = r;
	}
	cout << "we have " << recipes.size() << " recipes" << endl;
}

void get_factories() {
	std::ifstream factory_fs("../data/factorio-data/factory.json");
	json j;
	factory_fs >> j;
	for (auto& elem : j.items()) {
		string k = elem.key();
		auto v = elem.value();
		Factory f = v.get<Factory>();
		f.name = k;
		factories[k] = f;
	}
	cout << "we have " << factories.size() << " factories" << endl;
}

void get_technologies() {
	std::ifstream technology_fs("../data/factorio-data/technology.json");
	json j;
	technology_fs >> j;
	for (auto& elem : j.items()) {
		string k = elem.key();
		auto v = elem.value();
		Technology t = v.get<Technology>();
		t.name = k;
		technologies[k] = t;
	}
	cout << "we have " << technologies.size() << " technologies" << endl;
}

void get_settings(string settings_path) {
	std::ifstream settings_fs(settings_path);
	json j;
	settings_fs >> j;
	json initial_items = j.at("initial-items");
	for (auto& elem : initial_items.items()) {
		string k = elem.key();
		json v = elem.value();
		items.at(v.at("name")).amount += (int) v.at("amount");
	}
	json initial_factories = j.at("initial-factories");
	for (auto& elem : initial_factories.items()) {
		string k = elem.key();
		json v = elem.value();
		Factory f = factories[v.at("factory-type")];
		f.id = v.at("factory-id");
		Factory::ids = max(f.id + 1, Factory::ids);
		current_factories.push_back(f);
	}
	json goal = j.at("goal-items");
	for (auto& elem : goal.items()) {
		string k = elem.key();
		json v = elem.value();
		goal_items[v.at("name")] = items.at(v.at("name"));
		goal_items[v.at("name")].amount = (int) v.at("amount");
		cout << goal_items[v.at("name")].amount << " " << items[v.at("name")].amount << endl;
	}
}

int main(int argc, char** argv) {
	// redirect all the logs to the_void
	streambuf* old = cout.rdbuf();
	stringstream the_void;
	cout.rdbuf(the_void.rdbuf());
	// read the input from the given json files
	string challenge = "6";
	string settings_path = "../data/factorio-data/default-settings.json";
	settings_path = "../data/factorio-simulator/inputs/challenge-" + challenge + ".json";
	string target_path = "../data/factorio-simulator/outputs/challenge-" + challenge + "-out.json";
	if (argc < 3) {
		//cerr << "Please call the generator in this pattern:" << endl
		//	 << "./generator.sh -t target.json" << endl;
	} else {
		if (string(argv[1]) == "-t") {
			settings_path = argv[2];
			settings_path = "../"+settings_path;
		}
		//target_path = "../taget.json";
		//cout << argv[1] << " " << argv[2] << endl;
		//cerr << "the settings are: " << settings_path << endl;
	}


	get_items();
	get_recipes();
	for (auto& elem : recipes) {
		string k = elem.first;
		Recipe v = elem.second;
		for (auto& prod : v.products) {
			if (items.at(prod.first).recipe != "") {
				if (recipes[items.at(prod.first).recipe].category == "basic-smelting") {
					if (recipes[v.name].category == "basic-smelting") {
						if (v.ingredients.size() < recipes[items.at(prod.first).recipe].ingredients.size()) {
							items.at(prod.first).recipe = v.name;
						}
					}
				} else if (recipes[items.at(prod.first).recipe].category == "smelting") {
					if (recipes[v.name].category == "basic-smelting") {
						items.at(prod.first).recipe = v.name;
					} else if (recipes[v.name].category == "smelting") {
						if (v.ingredients.size() < recipes[items.at(prod.first).recipe].ingredients.size()) {
							items.at(prod.first).recipe = v.name;
						}
					}
				} else if (recipes[items.at(prod.first).recipe].category == "advanced-smelting") {
					if (recipes[v.name].category == "basic-smelting" || recipes[v.name].category == "smelting") {
						items.at(prod.first).recipe = v.name;
					} else if (recipes[v.name].category == "advanced-smelting") {
						if (v.ingredients.size() < recipes[items.at(prod.first).recipe].ingredients.size()) {
							items.at(prod.first).recipe = v.name;
						}
					}
				} else if (recipes[items.at(prod.first).recipe].name.find("empty") != string::npos) {
					items.at(prod.first).recipe = v.name;
				} else if (v.name.find("empty") != string::npos) {
					// -> skip
				} else if (v.ingredients.size() < recipes[items.at(prod.first).recipe].ingredients.size()) {
					items.at(prod.first).recipe = v.name;
				}
			} else {
				items.at(prod.first).recipe = v.name;
			}
		}
	}

	get_factories();

	get_technologies();

	for (auto& elem: technologies) {
		for (auto& rec : elem.second.recipes_unlocked) {
			recipes[rec].enabled_by = elem.first;
		}
	}
	get_settings(settings_path);

// create the requested item
	int time = 0;
	for (auto& elem : goal_items) {
		targets.push_back(pair<string, int>(elem.first, elem.second.amount));
	}
	while (!targets.empty()) {
		items[targets[targets.size() - 1].first].craft(targets[targets.size() - 1].second);
	}

	//cerr << "done" << endl;
	create_victory_event();


	// write to stdout
	cout.rdbuf(old);
	cout << setw(2) << Event::build_order << endl;

	// do we still need a target json file? because of the feedback message:
	// Option settings: The file '../target.json' doesn't exist. It should be the path of a valid JSON file.
	// enjoy the spam i guess?
	 ofstream output(target_path);
	 output << setw(2) << Event::build_order << endl;

	// ofstream output_2("../../target.json");
	// output_2 << setw(2) << Event::build_order << endl;
	return 0;
}
