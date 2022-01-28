
#ifndef FACTORIO_TECHNOLOGY_H
#define FACTORIO_TECHNOLOGY_H

#include <string>
#include <vector>
#include "json.hpp"

using nlohmann::json;

class Technology {
public:
	std::string name;
	std::vector<std::string> prerequisites;
	std::vector<std::pair<std::string, int>> ingredients;
	std::vector<std::string> recipes_unlocked;
	bool researched = false;

	void research();
};

inline void from_json(const json& j, Technology& t) {
	auto effects = j.at("effects");
	for (auto& elem : effects.items()) {
		std::string k = elem.key();
		json v = elem.value();
		t.recipes_unlocked.push_back(v.at("recipe"));
	}
	auto ingredients = j.at("ingredients");
	for (auto& elem : ingredients.items()) {
		std::string k = elem.key();
		json v = elem.value();
		t.ingredients.push_back(std::pair<std::string, int>(v.at("name"), v.at("amount")));
	}
	auto prerequisites = j.at("prerequisites");
	for (auto& elem : prerequisites.items()) {
		std::string k = elem.key();
		json v = elem.value();
		t.prerequisites.push_back(v);
	}
}

#endif //FACTORIO_TECHNOLOGY_H
