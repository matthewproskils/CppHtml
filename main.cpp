/*
    Compile + Run: gcc -o cpprun main.cpp -lstdc++ && cpprun
*/

#include <iostream>
#include <vector>
#include <fstream>
#include "htmlclass.hpp"
#include "parser.hpp"

string ReadFile(std::string name) {
	std::fstream file = std::fstream(name);

	string text = "";

	while (file.good())
		text += file.get();

	file.close();

	return text.substr(0, text.length() - 1);
}

int main() {
	dom document;
	document = parsefromstring(ReadFile("index.html"));
	std::cout << document.el[0].el[1].el[0].el[0].el[0].el[0].text;
	return 0;
}