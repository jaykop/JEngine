#pragma once
#include <component.hpp>

jeBegin

struct Telegram;

class Behavior : public Component {

	friend class Object;
	friend class BehaviorSystem;

public:

	// virtual void load() = 0;
	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void close() = 0;
	// virtual void unload() = 0;

protected:

	void add_to_system() override;
	void remove_from_system() override;
	virtual void load(const rapidjson::Value& /*data*/) {};

	Behavior(Object* owner) : Component(owner) {};
	virtual	~Behavior() {};

private:

};

jeEnd
