#pragma once
#include "pch.h"
#include <map>

#include "timer.hpp"

jeBegin

class Object;

jeDeclareUserComponentBuilder(ScriptController);

class ScriptController : public Behavior {

public:

	struct ScriptInfo
	{
		int id = -1;
		const wchar_t* txt = nullptr;
		int prev = -1;
		std::vector<int> next;
	};

	enum class SCRIPT_MODE {FLOW, CONTROL, AUTO_FLOW, LOCKED};
	enum class FADE_MODE {NONE, FADE_IN, FADE_OUT};

	ScriptController(Object* owner) : Behavior(owner) {};
	virtual ~ScriptController();

	void init() override;
	void update(float dt) override;
	void close() override;

	SCRIPT_MODE scriptMode_ = SCRIPT_MODE::FLOW;
	FADE_MODE fadeMode_ = FADE_MODE::NONE;

	float scriptSpeed = 1.f, fadeSpeed = 10.f;
	float alpha_ = 0.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	ScriptController& operator=(const ScriptController& rhs);
	ScriptController() = delete;

	void refresh_buffer();
	void script_renderer();
	void set_invisible();

	int current = 0, index_ = 0;
	Text* text_ = nullptr;
	Sprite* sprite_ = nullptr;
	Transform* transform_ = nullptr;
	Timer scriptTimer_, fadeTimer_;

	std::wstring txt_;
	std::map<int, ScriptInfo> scripts_;
};

jeEnd
