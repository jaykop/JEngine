/******************************************************************************/
/*!
\file   object_manager.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the definition of ObjectManager class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <unordered_map>
#include <string>

jeBegin

class Object;
using ObjectMap = std::unordered_map<const char*, Object*>;

class ObjectManager {

	// Prevent to clone this class
	ObjectManager() = delete;
	~ObjectManager() = delete;

	jePreventClone(ObjectManager)

	friend class Scene;
	friend class Object;

public:

	static Object* create_object(const char* name);
	static void remove_object(const char* name);
	static void remove_object(Object* object);
	static Object* get_object(const char* name);
	static bool has_object(const char* name);
	static void clear_objects();

	static ObjectMap* get_objects();

private:

	static int assign_id();

	static int idGenerator_;
	static ObjectMap* objects_;

};

jeEnd