
#ifndef FACTORIO_FACTORY_H
#define FACTORIO_FACTORY_H

#include <string>
#include <unordered_map>
#include "json.hpp"

using nlohmann::json;

class Factory {
public:
	std::string name;
	double speed = 1;
	std::unordered_map<std::string, bool> categories;
	int id = -1;
	static int ids;

	void craft();
	static void build(Factory f);

};

inline void from_json(const json& j, Factory& f) {
	j.at("crafting_speed").get_to(f.speed);
	auto categories = j.at("crafting_categories");
	for (auto& elem : categories.items()) {
		std::string k = elem.key();
		json v = elem.value();
		f.categories[k] = v;
	}
}


#endif //FACTORIO_FACTORY_H
