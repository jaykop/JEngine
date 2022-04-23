#pragma once

#include "colors.hpp"
#include <vector>
#include <map>

jeBegin

struct EventInfo
{
	enum class EVENT_STATUS { NONE, DONE, FAILED, PROGRESSING };

	EVENT_STATUS eventStatus = EVENT_STATUS::NONE;

	int id = -1, prev = -1;
	float weight = 1.f;

	std::vector<int> next;
	std::multimap<int, int> condition;
	std::vector<ScriptInfo> scripts;
};

struct ScriptInfo
{
	enum class SCRIPT_MODE { FLOW, CONTROL, AUTO_FLOW, LOCKED };
	enum class FADE_MODE { NONE, FADE_IN, FADE_OUT };

	SCRIPT_MODE scriptMode = SCRIPT_MODE::FLOW;
	FADE_MODE fadeMode = FADE_MODE::NONE;

	const wchar_t* script = nullptr;
	float scriptSpeed = 1.f, fadeSpeed = 1.f;
	vec3 color = Color::white;
};

jeEnd