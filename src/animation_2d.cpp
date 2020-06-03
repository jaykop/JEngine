#include <animation_2d.hpp>
#include <renderer.hpp>
#include <object.hpp>

jeBegin

jeDefineComponentBuilder(Animation2D);

Animation2D::Animation2D(Object* owner)
	: Component(owner), currentFrame_(0.f), animationSpeed_(0.f),
	frames_(1), realSpeed_(0.f), realFrame_(1.f), activated_(false)
{
	if (owner->has_component<Renderer>())
		owner->get_component<Renderer>()->animation_ = this;

	else
		jeDebugPrint("!Animation - This object has no renderer componnet: %s\n", owner->get_name());
}

Animation2D::~Animation2D() {

	// Turn off the toggle
	if (get_owner()->has_component<Renderer>())
		get_owner()->get_component<Renderer>()->animation_ = nullptr;
}

bool Animation2D::is_activated() const
{
	return activated_;
}

void Animation2D::activate(bool toggle)
{
	activated_ = toggle;
	if (activated_) timer_.start();
}

void Animation2D::fix_frame(int thFrame)
{
	animationSpeed_ = 0.f;
	activated_ = false;
	currentFrame_ = float(thFrame) * realFrame_;
}

void Animation2D::set_frame(int numOfFrame)
{
	frames_ = numOfFrame;
	realFrame_ = 1.f / float(frames_);
}

int Animation2D::get_frame() const
{
	return frames_;
}

void Animation2D::set_speed(float speed)
{
	animationSpeed_ = speed;
	realSpeed_ = 1.f / speed;
}

float Animation2D::get_speed() const
{
	return animationSpeed_;
}

jeEnd
