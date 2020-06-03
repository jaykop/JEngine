/******************************************************************************/
/*!
\file   component_manager.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the inline methods of ComponentManager class
*/
/******************************************************************************/

#pragma once
#include <typeinfo>
#include <macros.hpp>
#include <debug_tools.hpp>
#include <component_manager.hpp>

jeBegin

template <class ComponentType>
void ComponentManager::register_builder(
	const char* componentName, ComponentBuilder* builder)
{
	// get tyoe bane abd check if it's already registered
	const char* typeName = typeid(ComponentType).name();
	auto found = builderMap_.find(typeName);

	DEBUG_ASSERT(found == builderMap_.end(), "Trying to add existing component builder!");

	// add the builder
	builderMap_.insert(
		BuilderMap::value_type(typeName, builder));

	// add key and type name
	types_.insert(Directory::value_type(componentName, typeName));
	keys_.insert(Directory::value_type(typeName, componentName));
}
	
jeEnd