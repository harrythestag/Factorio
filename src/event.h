
#ifndef FACTORIO_EVENT_H
#define FACTORIO_EVENT_H

#include <string>
#include <vector>
#include "json.hpp"

using nlohmann::json;

class Event {
public:
	static json build_order;
	static int timestamp;
};

void create_research_event(std::string technology);

void create_build_factory_event(int factory_id, std::string factory_type, std::string factory_name);

void create_destroy_factory_event(int factory_id);

void create_start_factory_event(int factory_id, std::string recipe);

void create_stop_factory_event(int factory_id);

void create_victory_event();


#endif //FACTORIO_EVENT_H
