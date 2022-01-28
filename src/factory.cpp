
#include "factory.h"
#include "item.h"
#include "event.h"
#include <iostream>

using namespace std;

extern unordered_map<string, Item> items;
extern vector<Factory> current_factories;
extern int indentation;

int Factory::ids = 1;

void Factory::craft() {
	items[name].add_to_targets();
}

void Factory::build(Factory f) {
	// TODO: event
	indentation++;
	cout << string(indentation * 2, ' ') << "building a factory: " << f.name << endl;
	f.id = ids++;
	current_factories.push_back(f);

	create_build_factory_event(f.id, f.name, f.name);
	indentation--;
}
