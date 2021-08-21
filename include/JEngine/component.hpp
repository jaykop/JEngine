/******************************************************************************/
/*!
\file   component.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the definition of Component class
*/
/******************************************************************************/

#pragma once
#include <json_parser.hpp>

jeBegin

class Component {

	friend class Object;

public:

	Object* get_owner() const { return owner_; }

protected:

	virtual void add_to_system() = 0;
	virtual void remove_from_system() = 0;
	virtual void load(const rapidjson::Value& data) = 0;

	Component(Object* owner) : owner_(owner) {}
	virtual ~Component() {}

	Object* owner_ = nullptr;

private:

	bool bulitIn_ = false;

	// No default constructor
	Component() = delete;

};

jeEnd