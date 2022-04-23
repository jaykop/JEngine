#include "application.hpp"
#include "event_controller.h"
#include "game_scenes.h"
#include "sprite.hpp"
#include "text.hpp"
#include "script_controller.h"

#include <iostream>

jeBegin

jeDefineUserComponentBuilder(EventController);

EventController::~EventController()
{
	
}

void EventController::init()
{
	 currentEventId = statrtingEventId;
	 currentEvent = &events[currentEventId];
	 scriptController = new ScriptController;
}

void EventController::update(float dt)
{
	if (currentEvent->eventStatus == EventInfo::EVENT_STATUS::PROGRESSING)
	{
		scriptController;
	}
	else
		update_current_event();
}

void EventController::close() { }

void EventController::update_current_event()
{
	EventInfo* eventInfo = nullptr;

	const auto& next = currentEvent->next;
	if (next.size() == 1) eventInfo == &(events[next[0]]);
	else
	{
		float weightSum = 0;
		for (const auto& eventId : next)
		{
			weightSum += events[eventId].weight;
		}

		float rand = Random::get_rand_float(0.f, weightSum);
		for (const auto& eventId : next)
		{
			if (rand < events[eventId].weight)
				eventInfo = &events[eventId];

			rand -= events[eventId].weight;
		}
	}

	currentEvent = eventInfo;
	if (currentEvent != nullptr)
		currentEventId = currentEvent->id;

	else
		jeDebugPrint("!EventController - current event is null!");
}

void EventController::move_to_next_script()
{
	switch (scriptMode)
	{
	case ScriptInfo::SCRIPT_MODE::FLOW:
		if (InputHandler::key_triggered(KEY::SPACE))
		{
			if (!script[current].next.empty())
			{
				current = scripts[current].next[0];
			}
		}
		break;

	case ScriptInfo::SCRIPT_MODE::CONTROL:
		if (InputHandler::key_triggered(KEY::SPACE)
			|| InputHandler::key_triggered(KEY::RIGHT))
		{
			if (!script[current].next.empty())
			{
				current = scripts[current].next[0];
			}
		}

		if (InputHandler::key_triggered(KEY::BACK)
			|| InputHandler::key_triggered(KEY::LEFT))
		{
			int prev = script[current].prev;
			if (prev >= 0)
			{
				current = prev;
			}
		}
		break;

	case ScriptInfo::SCRIPT_MODE::AUTO_FLOW:

		if (!script[current].next.empty()
			&& scriptTimer.get_elapsed_time() >= 1.f)
		{
			current = scripts[current].next[0];

			scriptTimer.start();
		}

		break;

	case ScriptInfo::SCRIPT_MODE::LOCKED:
		break;
	}
}

jeEnd