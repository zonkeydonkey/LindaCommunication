#include "Scan.h"
#include <utility>

	Keywords::Keywords() {
		keywords.push_back(std::make_pair("output",output));
		keywords.push_back(std::make_pair("input",input));
		keywords.push_back(std::make_pair("read",read));
		keywords.push_back(std::make_pair("integer",integer));
		keywords.push_back(std::make_pair("string",string));
	}