#pragma once
#include "pch.h"
#include "script_info.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(EventController);

class EventController : public Behavior {

public:

	EventController(Object* owner) : Behavior(owner) {};
	virtual ~EventController();

	void init() override;
	void update(float dt) override;
	void close() override;

	void set_starting_event(int id) { statrtingEventId = id; }
	int get_starting_event_id() const { return statrtingEventId; }
	int get_current_event_id()  const { return currentEventId; }

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	EventController& operator=(const EventController& rhs);
	EventController() = delete;

	void update_current_event();
	void move_to_next_script();

	std::map<int, EventInfo> events;
	int currentEventId = -1, statrtingEventId = -1, 
		currentScriptIndex = 0, currentScriptCount = 0;
	EventInfo* currentEvent = nullptr;
	ScriptController* scriptController = nullptr;
	std::vector<ScriptInfo> currnetScripts;

};

jeEnd
