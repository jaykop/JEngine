/******************************************************************************/
/*!
\file   object.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the definition of Object class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <unordered_map>

jeBegin

class Object;
class Component;
using Children = std::unordered_map<const char*, Object*>;
using Components = std::unordered_map<const char*, Component*>;

class Object {

	friend class ObjectManager;

public:
	
	int get_id() const { return id_; }
	void register_components();

	const char* get_name();
	// void set_name(const char* name);

	Object* get_parent() { return parent_; }
	bool has_parent() { return parent_ != nullptr; }

	template <class ComponentType>	void add_component();
	template <class ComponentType>	ComponentType* get_component();
	template <class ComponentType>	bool has_component();
	template <class ComponentType>	void remove_component();

	void add_component(const char* componentName);
	Component* get_component(const char* componentName);
	bool has_component(const char* componentName);
	void remove_component(const char* componentName);

	void add_child(Object* child);
	void remove_child(const char* name);
	bool has_child(const char* name);
	Object* get_child(const char* name);

private:

	Object(const char* name);
	~Object();

	void set_parent(Object* parent) { parent_ = parent; }

	void clear_component();
	void clear_children();

	int id_ = -1;
	bool active_ = true;
	Object* parent_ = nullptr;
	const char* name_;
	Children children_;
	Components components_;

	Object() = delete;

};

jeEnd

#include <object.inl>