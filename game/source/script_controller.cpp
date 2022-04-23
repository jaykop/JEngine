#include "application.hpp"
#include "script_controller.h"
#include "game_scenes.h"
#include "sprite.hpp"
#include "text.hpp"

#include <iostream>

jeBegin

jeDefineUserComponentBuilder(ScriptController);

ScriptController::~ScriptController()
{
	remove_from_system();
}

void ScriptController::init()
{
	init_textbox();

	//ScriptInfo a = { L"으아아아아아아" };
	//ScriptInfo b = { L"말도 안된다구!" };
	//ScriptInfo c = { L"쯔아아아아아아아" };

	// set_invisible();

	scriptTimer.start();
	fadeTimer.start();
}

void ScriptController::update(float dt)
{
	float manipulativeDt = fadeTimer.get_elapsed_time();
	float& spriteAlpha = sprite->color.a;
	float& textAlpha = text->color.a;

	switch (fadeMode)
	{
		case ScriptInfo::FADE_MODE::FADE_IN:
			if (spriteAlpha < 1.f)
			{
				spriteAlpha += manipulativeDt;
				if (spriteAlpha >= 1.f)
					spriteAlpha = 1.f;
			}
			if (textAlpha < 1.f)
			{
				textAlpha += manipulativeDt;
				if (textAlpha >= 1.f)
				{
					textAlpha = 1.f;
					fadeMode = ScriptInfo::FADE_MODE::NONE;
				}
			}
			return;

		case ScriptInfo::FADE_MODE::FADE_OUT:
			if (spriteAlpha > 0.f)
			{
				spriteAlpha -= manipulativeDt;
				if (spriteAlpha <= 0.f)
					spriteAlpha = 0.f;
			}
			if (textAlpha > 0.f)
			{
				textAlpha -= manipulativeDt;
				if (textAlpha <= 0.f)
					textAlpha = 0.f;
			}
			return;

		case ScriptInfo::FADE_MODE::NONE:
			script_renderer();
			break;
	}
}

void ScriptController::close() { }

void ScriptController::refresh_buffer()
{
	txt.clear();
	index = 0;
}

void ScriptController::script_renderer()
{
	float scriptRenderDt = scriptTimer.get_elapsed_time();
	if (scriptRenderDt >= scriptSpeed)
	{
		if (currentText[index] != L'\0')
		{
			txt += currentText[index++];
		}
		scriptTimer.start();
	}

	text->set_text(txt.c_str());
}

void ScriptController::set_invisible()
{
	scriptMode = ScriptInfo::SCRIPT_MODE::LOCKED;
	fadeMode = ScriptInfo::FADE_MODE::FADE_IN;
	sprite->color.a = 0.f;
	text->color.a = 0.f;
}

void ScriptController::init_textbox()
{
	transform = owner_->get_component<Transform>();
	transform->position.y = -GraphicSystem::get_height() * 0.25;
	transform->scale.set(
		GraphicSystem::get_width() * 0.2f,
		GraphicSystem::get_height() * 0.1f,
		0);

	sprite = owner_->get_component<Sprite>();

	Object* txtChild = ObjectManager::create_object("ScriptText");
	Transform* childTransform = txtChild->get_component<Transform>();
	childTransform->scale.set(0.0015f, 0.005f, 1.f);
	childTransform->position.set(0, 0, 1.f);
	text = txtChild->add_component<Text>();
	text->set_font(AssetManager::get_font("default"));
	text->prjType = Renderer::ProjectType::ORTHOGONAL;

	owner_->add_child(txtChild);
	SceneManager::get_current_scene()->register_object(txtChild);
}

void ScriptController::set_current_script(ScriptInfo* scriptInfo)
{
	refresh_buffer();

	currentScript = scriptInfo;
	color = currentScript->color;
	scriptMode = currentScript->scriptMode;
	scriptSpeed = currentScript->scriptSpeed;
	fadeSpeed = currentScript->fadeSpeed;
	fadeMode = currentScript->fadeMode;

	currentText = currentScript->script;
}

void ScriptController::set_script_speed(float speed)
{
	if (speed <= 0) scriptSpeed = 0;
	else
	{
		scriptSpeed = 1.f / speed;
	}
}

float ScriptController::get_script_speed() const
{
	return scriptSpeed > 0 ? 1.f / scriptSpeed : 0;
}

jeEnd