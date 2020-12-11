#include "htmlclass.hpp"
#include <iostream>
#include <regex>

#pragma once

using std::string;

class error {
public:
	static void warning(string text) {
		std::cout << "WARNING - " + text;
	}
	static void err(string text) {
		std::cout << "ERROR - " + text;
	}
};

static htmlel replacel() {
	htmlel el;
	return el;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

static returnvalue getel(string html, int i, bool one) {
	int track = 0;
	dom document;
	error err;
	htmlel el;
	returnvalue returnthis;
	while (i < html.length()) {
		if (track == 0) {
			if (html[i] == '<') {
				el.istext = false;
				track = 1;
			}
			else {
				el.istext = true;
				el.text = html[i];
				track = 5;
			}
		}
		else if (track == 1) {
			if (html[i] == '>') {
				track++;
				if (el.tag == "") {
					err.warning("No Tag Specified");
				}
        if(el.tag[el.tag.length()-1] == '/'){
          document.el.push_back(el);
          returnthis.document = document;
					returnthis.i = i;
					return returnthis;
        }
			} else {
				el.tag += html[i];
			}
		}
		else if (track == 2) {
			if (html[i] == '<') {
				if (html[i + 1] == '/') {
					track++;
					i++;
				} else {
					returnvalue retval;
					retval = getel(html, i, true);
					if (retval.document.el[0].istext == false || retval.document.el[0].text != "") {
						el.el.push_back(retval.document.el[0]);
					}
					i = retval.i;
				}
			}
			else {
				returnvalue retval;
				retval = getel(html, i, true);
				el.el.push_back(retval.document.el[0]);
				i = retval.i;
			}
		}
		else if (track == 3) {
			if (html[i] == '>') {
				track = 0;
				if (one) {
					document.el.push_back(el);
					track = 0;
					returnthis.document = document;
					returnthis.i = i;
					return returnthis;
				}
				else {
					document.el.push_back(el);
					track = 0;
					el = replacel();
				}
			}
		}
		else if (track == 5) {
			if (html[i] == '<') {
				if (one) {
					i--;
					document.el.push_back(el);
					returnthis.document = document;
					returnthis.i = i;
					return returnthis;
				}
				else {
					track = 0;
					i--;
					if (std::regex_replace(el.text, std::regex("\\s*"), "") != "") {
						document.el.push_back(el);
					}
				}
			}
			else {
				el.text += html[i];
			}
		}
		i++;
	}
	returnthis.document = document;
	returnthis.i = i;
	return returnthis;
}

static dom parsefromstring(string html) {
	html = std::regex_replace(html, std::regex("\n*"),"");
	std::string oldhtml = html;
	while(true){
		html = std::regex_replace(html, std::regex("  "), "");
		if(html.length() == oldhtml.length()){
			break;
		}else{
			oldhtml = html;
		}
	}
	return getel(html, 0, false).document;
}