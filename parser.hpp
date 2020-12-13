#include "htmlclass.hpp"
#include <iostream>
#include <regex>

#pragma once

using std::string;
using std::map;

class returnvalue {
public:
	HTMLDocument* dom;
	int i;
};


map<string, HTMLNode*> ParseIdsNode(HTMLNode* dom, string attr) {
	map<string, HTMLNode*> ids = map<string, HTMLNode*>();
	if (dom->HasAttr(attr)) {
		ids[dom->FindAttr(attr)] =  dom;
	}
	for (size_t i = 0; i < dom->children.size(); i++) {
		map<string, HTMLNode*> parsed = ParseIdsNode(dom->children[i], attr);
		if (parsed.size() > 0) {
			ids[parsed.begin()->first] = parsed.begin()->second;
		}
	}
	return ids;
}

map<string, HTMLNode*> GetAttr(HTMLDocument* dom, string attr) {
	map<string, HTMLNode*> ids = map<string, HTMLNode*>();
	for (size_t i = 0; i < dom->children.size(); i++) {
		map<string, HTMLNode*> parsed = ParseIdsNode(dom->children[i], attr);
		if (parsed.size() > 0) {
			ids[parsed.begin()->first] = parsed.begin()->second;
		}
	}
	return ids;
}

returnvalue parsefromstring(string html, bool one, size_t i) {
	int track = 0;
	int trackattr = 0;
	returnvalue ret;
	std::pair<string, string> attr = std::pair<string, string>("", "");

	HTMLDocument* document = new HTMLDocument();
	HTMLNode* node = new HTMLNode("");
	while (i < html.length()) {
		if (track == 0) {
			if (html[i] == ' ' || html[i] == '\n' || html[i] == '	') {
				//Ignore
			}
			else if (html[i] == '<') {
				//Is HTML Tag
				track++;
				node = new HTMLNode("");
			}
			else {
				track = 5;

				node = new HTMLNode("text");
				node->istext = true;
				node->text = html[i];
			}
		}
		else if (track == 1) {
			if (html[i] == ' ' || html[i] == '	' || html[i] == '\n'){
				track++;
			}
			else if (html[i] == '>') {
				track = 3;
			}
			else {
				node->TagName += html[i];
			}
		}
		else if (track == 2) {
			if (html[i] == '>') {
				if (attr.first != "") {
					node->Attributes.insert(attr);
				}
				attr.first = "";
				attr.second = "";
				track++;
				if(html[i-1] == '/'){
					if (one) {
						document->Append(node);
						ret.dom = document;
						ret.i = i - 1;
						return ret;
					}
					else {
						document->Append(node);
					}
				}
			}
			else if (trackattr == 0) {
				if (html[i] == '=') {
					trackattr++;
				}
				else {
					attr.first += html[i];
				}
			}
			else if (trackattr == 1) {
				if (html[i] == ' ' || html[i] == '	' || html[i] == '\n') {
					//Ignore
				}
				else if (html[i] == '\"') {
					trackattr = 3;
				}
				else if (html[i] == '\'') {
					trackattr = 4;
				}
				else {
					trackattr = 2;
				}
			}
			else if (trackattr == 2) {
				//Non String
				if (html[i] == ' ') {
					if (attr.first != "") {
						node->Attributes.insert(attr);
					}
					attr.first = "";
					attr.second = "";
					trackattr = 0;
				}
				else {
					attr.second += html[i];
				}
			}
			else if (trackattr == 3) {
				if (html[i] == '\\') {
					attr.second += html[i];
					i++;
					attr.second += html[i];
				}
				else if (html[i] == '\"') {
					if (attr.first != "") {
						node->Attributes.insert(attr);
					}
					attr.first = "";
					attr.second = "";
					trackattr = 0;
				}
				else {
					attr.second += html[i];
				}
			}
			else if (trackattr == 4) {
				if (html[i] == '\\') {
					attr.second += html[i];
					i++;
					attr.second += html[i];
				}
				else if (html[i] == '\'') {
					if (attr.first != "") {
						node->Attributes.insert(attr);
					}
					attr.first = "";
					attr.second = "";
					trackattr = 0;
				}
				else {
					attr.second += html[i];
				}
			}
		}
		else if (track == 3) {
			if (html[i] == '<' && html[i + 1] == '/') {
				track++;
			}
			else {
				if (html[i] != ' ' && html[i] != '	' && html[i] != '\n') {
					returnvalue search = parsefromstring(html, true, i);
					if (search.dom->children.size() == 1) {
						if (search.dom->children[0]->istext == false || search.dom->children[0]->text != "") {
							node->Append(search.dom->children[0]);
							i = search.i;
						}
					}
				}
			}
		}
		else if (track == 4) {
			if (html[i] == '>') {
				if (one) {
					document->Append(node);
					ret.dom = document;
					ret.i = i;
					return ret;
				}
				else {
					document->Append(node);
					node = new HTMLNode("");
				}
			}
		}
		else if (track == 5) {
			if (html[i] == '\\') {
				node->text += html[i];
				i++;
				node->text += html[i];
			}
			else if (html[i] == '<') {
				if (one) {
					document->Append(node);
					ret.dom = document;
					ret.i = i - 1;
					return ret;
				}
				else {
					document->Append(node);
				}
			}
			else {
				node->text += html[i];
			}
		}
		i++;
	}
	ret.dom = document;
	ret.i = i;
	return ret;
}