/******************************************************************************/
/*!
\file   json_parser.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of JsonParser class
*/
/******************************************************************************/

#include <debug_tools.hpp>
#include <json_parser.hpp>
#include <stringbuffer.h>
#include <istreamwrapper.h>
#include <fstream>
#include <thread>

jeBegin

rapidjson::Document JsonParser::document_;

void JsonParser::read_file(const char* directory)
{
	std::ifstream read(directory);
	rapidjson::IStreamWrapper toInputStream(read);
#if defined(_DEBUG)
	DEBUG_ASSERT(!document_.ParseStream(toInputStream).HasParseError(), "Json file has a problem");
#else
	document_.ParseStream(toInputStream);
#endif // DEBUG

}

const rapidjson::Document& JsonParser::get_document() { return document_; }

void JsonParser::close() { document_.Clear();}

void JsonParser::load_objects()
{
	//CR_RJValue object = document_["Object"];

	//for (rapidjson::SizeType i = 0; i < object.Size(); ++i) {

	//	// Check either if object's name
	//	if (object[i]["Name"].IsString()) {
	//		//FACTORY::CreateObject(object[i]["Name"].GetString());
	//		std::thread object_thread(&FACTORY::CreateObject, object[i]["Name"].GetString());
	//		object_thread.join();

	//		// Check either if object has any component
	//		if (object[i].HasMember("Component")) {
	//			CR_RJValue component = object[i]["Component"];

	//			// Check either if components have correct type
	//			for (rapidjson::SizeType j = 0; j < component.Size(); ++j) {

	//				if (component[j].HasMember("Type")) {
	//					LoadComponents(component[j]);
	//					//std::thread component_thread(&LoadComponents, component[j]);
	//					//component_thread.join();
	//				}
	//				else
	//					jeDebugPrint("!JsonParser - Wrong component type or values.\n");
	//			}

	//		} // if (object[i].HasMember("Component")) {

	//		else
	//			jeDebugPrint("!JsonParser - No component in this object: %s\n", object[i]["Name"].GetString());

	//		//FACTORY::AddCreatedObject();
	//		std::thread register_thread(&FACTORY::AddCreatedObject);
	//		register_thread.join();
	//	} // if (object[i]["Name"].IsString()) {

	//	else
	//		jeDebugPrint("!JsonParser - Wrong type of object name.\n");

	//} // for (rapidjson::SizeType i = 0; i < object.Size(); ++i) {
}

void JsonParser::load_components(const rapidjson::Value& data)
{
	//FACTORY::GetCreatedObject()->AddComponent(data["Type"].GetString());
	//Component* found =
	//	FACTORY::GetCreatedObject()->GetComponent(data["Type"].GetString());

	//if (data.HasMember("Values"))
	//	found->Load(data["Values"]);
}

jeEnd