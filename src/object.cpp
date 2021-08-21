#pragma warning (disable : 4189)

/******************************************************************************/
/*!
\file   object.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the methods of Object class
*/
/******************************************************************************/

#include <object.hpp>
#include <component.hpp>
#include <debug_tools.hpp>
#include <object_manager.hpp>
#include <component_manager.hpp>

jeBegin

Object::Object(const char* name)
	:name_(name), active_(true), parent_(nullptr)
{
	id_ = ObjectManager::assign_id();
}

Object::~Object()
{
	clear_component();
	clear_children();
}

void Object::register_components()
{
	for (const auto& component : components_)
		component.second->add_to_system();
}

const char* Object::get_name() const
{
	return name_.c_str();
}

Component* Object::add_component(const char* componentName)
{
	const char* typeName = ComponentManager::key_to_type(componentName);
	const auto& found = components_.find(typeName);

	DEBUG_ASSERT(found == components_.end(), "Trying to add an existing component!");

	Component* newComponent = ComponentManager::create_component(typeName, this);
	components_.insert(Components::value_type(typeName, newComponent));

	return newComponent;
}

Component* Object::get_component(const char* componentName)
{
	const char* typeName = ComponentManager::key_to_type(componentName);
	const auto& found = components_.find(typeName);

	DEBUG_ASSERT(found != components_.end(), "No such name of component!");

	return found->second;
}

bool Object::has_component(const char* componentName)
{
	const char* typeName = ComponentManager::key_to_type(componentName);
	const auto& found = components_.find(typeName);

	return found != components_.end();
}

void Object::remove_component(const char* componentName)
{
	const char* typeName = ComponentManager::key_to_type(componentName);
	const auto& found = components_.find(typeName);

	DEBUG_ASSERT(found != components_.end(), "No such name of component!");

	delete found->second;
	found->second = nullptr;
}

void Object::add_child(Object* child)
{
	const auto& found = children_.find(child->get_name());
	DEBUG_ASSERT(found == children_.end(), "Trying to add an existing object!");
	children_.insert(Children::value_type(child->get_name(), child));
	child->set_parent(this);
}

void Object::remove_child(const char* name)
{
	ObjectManager::remove_object(name);
}

bool Object::has_child(const char* name)
{
	const auto& found = children_.find(name);
	return found != children_.end();
}

Object* Object::get_child(const char* name)
{
	const auto& found = children_.find(name);
	DEBUG_ASSERT(found != children_.end(), "No such child object!");
	return found->second;
}

void Object::clear_children()
{
	//for (auto child : children_)
	//	ObjectManager::remove_object(child.second);

	children_.clear();
}

void Object::clear_component()
{
	// Clear all components in the list
	for (auto& component : components_) {

		if (component.second) {
			delete component.second;
			component.second = nullptr;
		}
	}

	components_.clear();
}

jeEnd