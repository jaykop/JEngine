#pragma once
#include <cstring>
#include <macros.hpp>
#include <string_view>

jeBegin


struct string_comparator {


	std::string_view a;

	template<typename Pair>
	bool operator()(const Pair& p1, const Pair& p2) const {
		return !strcmp(p1.first, p2.first);
	}
};

jeEnd