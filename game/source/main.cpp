#include "JEngine.h"
#include "application.hpp"
#include "macros.hpp"
#include <debug_tools.hpp>

using namespace JE;

int main(int argc, char* args[]) {

	jeUnused(argc);	// No argc - Block the warnings
	jeUnused(args);	// No args - Block the warnings

	// pop a console window
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	// init user scene & game logics
	JEngine::register_assets();

	// if initialization succeeded, run the app
	if (Application::initialize()) {

		Application::update();
		Application::close();
	}

	// destroy the console window
	DEBUG_DESTROY_CONSOLE();

	return 0;
}