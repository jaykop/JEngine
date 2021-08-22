#include "application.hpp"
#include "script_controller.h"
#include "game_scenes.h"
#include "sprite.hpp"
#include "text.hpp"

jeBegin

jeDefineUserComponentBuilder(ScriptController);

ScriptController::~ScriptController()
{
	remove_from_system();
}

void ScriptController::init()
{
	transform_ = owner_->get_component<Transform>();
	transform_->scale.set(1,1,0);

	Object* txtChild = ObjectManager::create_object("ScriptText");
	Transform* childTransform = txtChild->get_component<Transform>();
	childTransform->scale.set(.75f, .75f, 1.f);
	childTransform->position.set(-0.45f * GraphicSystem::get_width(), -0.3f * GraphicSystem::get_height(), 1.f);
	text_ = txtChild->add_component<Text>();
	text_->set_font(AssetManager::get_font("default"));
	text_->prjType = Renderer::ProjectType::ORTHOGONAL;

	owner_->add_child(txtChild);
	SceneManager::get_current_scene()->register_object(txtChild);

	ScriptInfo a = { 0 , L"으아아아아아아", -1, {1} };
	ScriptInfo b = { 1 , L"말도 안된다구!", -1, {2} };
	ScriptInfo c = { 2 , L"쯔아아아아아아아", -1, {} };

	scripts_.insert({ 0, a });
	scripts_.insert({ 1, b });
	scripts_.insert({ 2, c });
}

void ScriptController::update(float dt)
{
	float manipulativeDt = dt / timer_;
	float& spriteAlpha = sprite_->color.a;
	float& textAlpha = text_->color.a;

	switch (fadeMode_)
	{
		case FADE_MODE::FADE_IN:
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
					textAlpha = 1.f;
			}
			return;

		case FADE_MODE::FADE_OUT:
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

		case FADE_MODE::NONE:
			break;
	}

	switch (scriptMode_)
	{
	case SCRIPT_MODE::FLOW:
		if (InputHandler::key_triggered(KEY::SPACE))
		{
			if (!scripts_[current].next.empty())
			{
				refresh_buffer();
				current = scripts_[current].next[0];
			}
		}
		break;

	case SCRIPT_MODE::CONTROL:
		if (InputHandler::key_triggered(KEY::SPACE)
			|| InputHandler::key_triggered(KEY::RIGHT))
		{
			if (!scripts_[current].next.empty())
			{
				refresh_buffer();
				current = scripts_[current].next[0];
			}
		}

		if (InputHandler::key_triggered(KEY::BACK)
			|| InputHandler::key_triggered(KEY::LEFT))
		{
			int prev = scripts_[current].prev;
			if (prev >= 0)
			{
				refresh_buffer();
				current = prev;
			}
		}
		break;
	
	case SCRIPT_MODE::AUTO_FLOW:
		break;

	case SCRIPT_MODE::LOCKED:
		break;
	}

	if (scripts_[current].txt[index_] != '\0')
	{
		txt_ += scripts_[current].txt[index_++];
	}
	text_->set_text(txt_.c_str());
}

void ScriptController::refresh_buffer()
{
	txt_.clear();
	index_ = 0;
}

void ScriptController::close() { }

jeEnd