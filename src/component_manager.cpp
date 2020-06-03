/******************************************************************************/
/*!
\file   component_manager.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the methods of ComponentManager class
*/
/******************************************************************************/
#include <component_manager.hpp>
#include <debug_tools.hpp>
#include <component_builder.hpp>

jeBegin

BuilderMap ComponentManager::builderMap_;
Directory ComponentManager::keys_, ComponentManager::types_;

Component* ComponentManager::create_component(const char* componentName,
	Object* owner) {
	
	// find component builder
	auto found = builderMap_.find(componentName);
	if (found == builderMap_.end()) {
		jeDebugPrint("No such name of component");
		return nullptr;
	}
	
	// return new component created
	return found->second->create_component(owner);
} 

const char* ComponentManager::key_to_type(const char* name)
{
	auto found = types_.find(name);
	DEBUG_ASSERT(found == types_.end(), "No such name of conmponent");
	return found->second.c_str();

}

const char* ComponentManager::type_to_key(const char* type)
{
	auto found = keys_.find(type);
	if (found == keys_.end()) {
		jeDebugPrint("No such name of conmponent");
		return nullptr;
	}
	return found->second.c_str();
}

void ComponentManager::clear_builders()
{
	// clear instances
	for (auto it = builderMap_.begin();
		it != builderMap_.end();)
		delete (it++)->second;

	// clear containers
	builderMap_.clear();
	keys_.clear();
	types_.clear();
}

jeEnd