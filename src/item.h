#ifndef FACTORIO_ITEM_H
#define FACTORIO_ITEM_H


#include <string>
#include "json.hpp"

using nlohmann::json;

class Item {
public:
	std::string name;
	std::string type;
	int amount = 0;
	std::string recipe = "";

	Item(std::string name, std::string type);
	Item();

	void craft(int num = 1);

	void add_to_targets(int num = 1);

};

//void from_json(const json& j, Item& i) {
//
//}

#endif //FACTORIO_ITEM_H
