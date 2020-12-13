#pragma once

string ReadFile(std::string name) {
	std::fstream file = std::fstream(name);

	string text = "";

	while (file.good())
		text += file.get();

	file.close();

	return text.substr(0, text.length() - 1);
}

