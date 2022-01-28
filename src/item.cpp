#include "item.h"
#include "recipe.h"
#include <string>
#include <iostream>

using namespace std;

extern unordered_map<string, Recipe> recipes;
extern vector<pair<string, int>> targets;
extern int indentation;


Item::Item(std::string name, std::string type) : name(name), type(type) {

}

Item::Item() : name(""), type("") {

}

void Item::craft(int num) {
	indentation++;
	if (amount < num) {
		cout << string(indentation * 2, ' ') << "crafting " << num << " " << name << endl;
		recipes[recipe].execute(num);
	} else {
		targets.pop_back();
	}
	indentation--;

}

void Item::add_to_targets(int num) {
	targets.push_back(pair<string, int>(name, num));
}
