#pragma once
#include "pch.h"

#include "timer.hpp"
#include "script_info.h"

jeBegin

class Object;

jeDeclareUserComponentBuilder(ScriptController);

class ScriptController : public Behavior {

public:

	ScriptController(Object* owner) : Behavior(owner) {};
	virtual ~ScriptController();

	void init() override;
	void update(float dt) override;
	void close() override;

	void set_current_script(ScriptInfo* scriptInfo);

	void set_script_speed(float speed);

	float get_script_speed() const;

	ScriptInfo::SCRIPT_MODE scriptMode = ScriptInfo::SCRIPT_MODE::FLOW;
	ScriptInfo::FADE_MODE fadeMode = ScriptInfo::FADE_MODE::NONE;

	vec3 color = Color::white;

	float scriptSpeed = 1.f, fadeSpeed = 10.f;
	float alpha = 0.f;

protected:

	void load(const rapidjson::Value& /*data*/) override {};

private:

	ScriptController& operator=(const ScriptController& rhs);
	ScriptController() = delete;

	void init_textbox();

	void refresh_buffer();
	void script_renderer();
	void set_invisible();

	int current = 0, index = 0;
	Text* text = nullptr;
	Sprite* sprite = nullptr;
	Transform* transform = nullptr;
	Timer scriptTimer, fadeTimer;

	const wchar_t* currentText = nullptr;

	ScriptInfo* currentScript = nullptr;
	std::wstring txt;
};

jeEnd
