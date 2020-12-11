#include <iostream>
#include <vector>

#pragma once

class arguments {
public:
	std::string id = "";
	std::vector< std::string > classes = {};
	std::string href = "";
	std::string rel = "";
};

class htmlel {
	public:
		std::string tag = "";
		bool istext = false;
		std::vector<htmlel> el = {};
		std::string text = "";
		arguments args;
};

class dom {
public:
	std::vector<htmlel> el = {};
};

class returnvalue {
	public:
		int i = 0;
		dom document;
};