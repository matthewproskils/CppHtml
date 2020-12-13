#include <vector>
#include <iostream>
#include <map>

#pragma once

using std::string;
using std::map;
using std::vector;

class HTMLNode {
public:
	HTMLNode* parent;
	
	bool istext = false;

	string text;

	vector<HTMLNode*> children = vector<HTMLNode*>();

	map<string, string> Attributes = map<string, string>();

	HTMLNode* GetChild(size_t index) {
		if (index >= children.size()) {
			return nullptr;
		}
		else {
			return children[index];
		}
	}

	string FindAttr(string name) {
		return Attributes.find(name)->second;
	}

	std::pair < string, string > GetAttr(size_t index) {
		if (Attributes.size() >= index) {
			return std::make_pair("", "");
		}
			else {
			return std::make_pair(std::next(Attributes.begin(), 4)->first, std::next(Attributes.begin(), 4)->second);
		}
	}

	void Append(HTMLNode* node) {
		children.push_back(node);

		node->parent = this;
	}

	bool HasAttr(string attr) {
		return Attributes.find(attr) != Attributes.end();
	}

	string innerText() {
		string text = "";
		for (size_t i = 0; i < children.size(); i++) {
			if (children[i]->istext) {
				text += children[i]->text;
			}
		}
		return text;
	}

	HTMLNode (string tagname) : TagName(tagname) {};

	string TagName;
};

class HTMLDocument : public HTMLNode {
public:
	HTMLDocument() : HTMLNode("document") {}
};