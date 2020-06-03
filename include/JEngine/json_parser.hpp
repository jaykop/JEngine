/******************************************************************************/
/*!
\file   json_parser.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the definition of JsonParser class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <document.h>

jeBegin

class JsonParser {

	// Prevent to clone this class
	JsonParser() = delete;
	~JsonParser() = delete;

	jePreventClone(JsonParser)

	friend class Scene;
	friend class Application;
	friend class AssetManager;

private:

	static void	read_file(const char* directory);
	static const rapidjson::Document& get_document();
	static void	close();

	static void load_objects();
	static void load_components(const rapidjson::Value& data);

	static rapidjson::Document document_;

};

jeEnd
