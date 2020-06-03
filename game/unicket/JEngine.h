#pragma once
#include "application.hpp"

class JEngine
{
public:

	static void register_builders();
	static void register_scenes();
	static bool register_assets();
	static void run();

};
