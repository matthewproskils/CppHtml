/*
Compile + Run: gcc -o cpprun main.cpp -lstdc++ && cpprun
*/

#include <iostream>
#include <vector>
#include <fstream>	
#include "htmlclass.hpp"
#include "parser.hpp"
#include "readfile.hpp"


int main() {
	HTMLDocument*document = parsefromstring(ReadFile("index.html"), false, 0).dom;

	map<string, HTMLNode*> ids = GetAttr(document, "id");

	std::cout << ids["hi"]->innerText();
	
	return 0;
}