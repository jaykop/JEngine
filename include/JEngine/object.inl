/******************************************************************************/
/*!
\file   object.inl
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the inline methods of Object class
*/
/******************************************************************************/

#pragma once
#include <object.hpp>
#include <component_manager.hpp>

jeBegin

template <class ComponentType>
ComponentType* Object::add_component() {

	const char* typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);
	if (found != components_.end()) {
		jeDebugPrint("[Object::add_component] Trying to add an existing component!\n");
		return nullptr;
	}

	Component* newComponent = ComponentManager::create_component(typeName, this);
	components_.insert(Components::value_type(typeName, newComponent));

	return reinterpret_cast<ComponentType*>(newComponent);
}

template <class ComponentType>
ComponentType* Object::get_component() {

	const char* typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);

	if (found == components_.end()) {
		jeDebugPrint("[Object::get_component] No such name of component!\n");
		return nullptr;
	}

	return reinterpret_cast<ComponentType*>(found->second);
}

template <class ComponentType>
bool Object::has_component() {

	const char* typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);
	
	return found != components_.end();
}

template <class ComponentType>
void Object::remove_component() {

	const char* typeName = ComponentManager::key_to_type(typeid(ComponentType).name());
	auto found = components_.find(typeName);

	DEBUG_ASSERT(found != components_.end(), "[Object::remove_component] No such name of component!\n");

	delete found->second;
	found->second = nullptr;
}

jeEnd