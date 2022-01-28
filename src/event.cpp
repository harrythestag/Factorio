
#include "event.h"

json Event::build_order = json::array();
int Event::timestamp = 0;

void create_research_event(std::string technology) {
	json e;
	e["timestamp"] = 1 + Event::timestamp;
	Event::timestamp += 2;
	e["technology"] = technology;
	e["type"] = "research-event";
	Event::build_order.push_back(e);
}

void create_build_factory_event(int factory_id, std::string factory_type, std::string factory_name) {
	json e;
	e["timestamp"] = Event::timestamp;
	e["factory-id"] = factory_id;
	e["factory-type"] = factory_type;
	e["factory-name"] = factory_name;
	e["type"] = "build-factory-event";
	Event::build_order.push_back(e);
}

void create_destroy_factory_event(int factory_id) {
	json e;
	e["timestamp"] = Event::timestamp;
	e["factory-id"] = factory_id;
	e["type"] = "destroy-factory-event";
	Event::build_order.push_back(e);
}

void create_start_factory_event(int factory_id, std::string recipe) {
	json e;
	e["timestamp"] = Event::timestamp;
	e["factory-id"] = factory_id;
	e["recipe"] = recipe;
	e["type"] = "start-factory-event";
	Event::build_order.push_back(e);
}

void create_stop_factory_event(int factory_id) {
	json e;
	e["timestamp"] = Event::timestamp;
	e["factory-id"] = factory_id;
	e["type"] = "stop-factory-event";
	Event::build_order.push_back(e);
}

void create_victory_event() {
	json e;
	e["timestamp"] = Event::timestamp;
	e["type"] = "victory-event";
	Event::build_order.push_back(e);
}