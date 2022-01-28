
#ifndef FACTORIO_RECIPE_H
#define FACTORIO_RECIPE_H


#include <string>
#include <vector>
#include "json.hpp"

using nlohmann::json;


class Recipe {
public:
	std::string name;
	std::string category;
	bool enabled = false;
	int energy = 0;
	std::vector<std::pair<std::string, int>> ingredients;
	std::vector<std::pair<std::string, int>> products;
	std::string enabled_by;

	void execute(int num = 1);

};

inline void from_json(const json& j, Recipe& r) {
	j.at("category").get_to(r.category);
	j.at("enabled").get_to(r.enabled);
	j.at("energy").get_to(r.energy);
	auto ingredients = j.at("ingredients");
	for(auto& elem : ingredients.items()){
		std::string k = elem.key();
		json v = elem.value();
		r.ingredients.push_back(std::pair<std::string, int>(v.at("name"), v.at("amount")));
	}
	auto products = j.at("products");
	for(auto& elem : products.items()){
		std::string k = elem.key();
		json v = elem.value();
		r.products.push_back(std::pair<std::string, int>(v.at("name"), v.at("amount")));
	}
}


#endif //FACTORIO_RECIPE_H
