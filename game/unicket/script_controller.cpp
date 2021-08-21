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
	// sprite_ = owner_->add_component<Sprite>();
	text_ = owner_->add_component<Text>();
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
				current = scripts_[current].next[0];
		}
		break;

	case SCRIPT_MODE::CONTROL:
		if (InputHandler::key_triggered(KEY::SPACE)
			|| InputHandler::key_triggered(KEY::RIGHT))
		{
			if (!scripts_[current].next.empty())
				current = scripts_[current].next[0];
		}

		if (InputHandler::key_triggered(KEY::BACK)
			|| InputHandler::key_triggered(KEY::LEFT))
		{
			int prev = scripts_[current].prev;
			if (prev >= 0)
				current = prev;
		}
		break;
	
	case SCRIPT_MODE::AUTO_FLOW:
		break;

	case SCRIPT_MODE::LOCKED:
		break;
	}

	text_->set_text(scripts_[current].txt);
}

void ScriptController::close() { }

jeEnd