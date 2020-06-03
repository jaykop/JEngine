/******************************************************************************/
/*!
\file   component_builder.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the definition of ComponentBuilder class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

class Object;
class Behavior;
class Component;

class ComponentBuilder {
	
	// Prevent to clone this class
	jePreventClone(ComponentBuilder)

	friend class ComponentManager;

protected:

	ComponentBuilder() {};
	virtual ~ComponentBuilder() {};

private:

	virtual Component* create_component(Object* owner) const = 0;

};

#define jeBaseFriends(c)				\
	friend class jeConcat(c, Builder);	\
	friend class ComponentManager;
#define jeConcat(a, b)			a ## b
#define jeDefineComponentBuilder(c)					\
	jeConcat(c, Builder)::jeConcat(c, Builder)() {} \
	Component* jeConcat(c, Builder)::create_component(Object* owner) const { return new (c)(owner);}
#define jeDeclareComponentBuilder(c)	\
	class jeConcat(c, Builder) : public ComponentBuilder { \
	friend class AssetManager; \
	jeConcat(c, Builder)(); \
	~jeConcat(c, Builder)() {}; \
	jeConcat(c, Builder)(const jeConcat(c, Builder)& /*copy*/) = delete; \
	jeConcat(c, Builder)& operator=(const jeConcat(c, Builder)& /*copy*/) = delete; \
	jeConcat(c, Builder)(jeConcat(c, Builder) && /*copy*/) = delete; \
	jeConcat(c, Builder)& operator=(jeConcat(c, Builder) && /*copy*/) = delete; \
	Component* create_component(Object* owner) const override; \
	}
#define jeDefineUserComponentBuilder(c)	\
	jeConcat(c, Builder)::jeConcat(c, Builder)() {} \
	Behavior* jeConcat(c, Builder)::create_component(Object* owner) const { return new (c)(owner); } 
#define jeDeclareUserComponentBuilder(c)	\
	class jeConcat(c, Builder) : public ComponentBuilder { \
	friend class JEngine; \
	jeConcat(c, Builder)(); \
	~jeConcat(c, Builder)() {}; \
	jeConcat(c, Builder)(const jeConcat(c, Builder)& /*copy*/) = delete; \
	jeConcat(c, Builder)& operator=(const jeConcat(c, Builder)& /*copy*/) = delete; \
	jeConcat(c, Builder)(jeConcat(c, Builder) && /*copy*/) = delete; \
	jeConcat(c, Builder)& operator=(jeConcat(c, Builder) && /*copy*/) = delete; \
	Behavior* create_component(Object* owner) const override; \
	}

jeEnd