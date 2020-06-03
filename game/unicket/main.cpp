#include "JEngine.h"
#include "macros.hpp"

int main(int argc, char* args[]) {

	jeUnused(argc);	// No argc - Block the warnings
	jeUnused(args);	// No args - Block the warnings

	if (JEngine::register_assets())
		JEngine::run();

	return 0;
}