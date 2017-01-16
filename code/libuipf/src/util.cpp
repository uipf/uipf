#include "util.hpp"

#include <vector>
#include <algorithm>


// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
/*
s	old name, which has to be renamed
*/
std::string uipf::util::rename(const std::string& s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, '.')) {
		tokens.push_back(token);
	}
	if (tokens.size() < 1) {
		return "";
	} else if (tokens.size() == 1) {
		std::string end = tokens[0];
		tokens.pop_back();
		tokens.push_back(end + "_result");
	} else {
		std::string end = tokens[tokens.size() - 1];
		tokens.pop_back();
		std::string beforeEnd = tokens[tokens.size() - 1];
		tokens.pop_back();
		beforeEnd = beforeEnd + "_result";
		tokens.push_back(beforeEnd);
		tokens.push_back(end);
	}

	std::string newName;

	newName = tokens[0];
	for (unsigned int i=1; i<tokens.size(); i++){
		newName = newName +"." + tokens[i];
	}

	return newName;
}

// gets the second part of the string, which is divided by : , e.g. source:../ball.png -> ../ball.png
/*
s	complete string, or only the second part
*/
std::string uipf::util::secondPart(const std::string& s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, ':')) {
		if (!token.empty())
			tokens.push_back(token);
	}

	if(tokens.size() == 1){
		return tokens[0];
	} else{
		return tokens[1];
	}
}

// gets the first part of the string, which is divided by : , e.g. source:../ball.png -> source
/*
s	complete string
*/
std::string uipf::util::firstPart(const std::string& s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, ':')) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens[0];
}

bool uipf::util::str_begins_with(const std::string& s, const std::string& with) {
	if (s.length() >= with.length()) {
		return s.compare(0, with.length(), with) == 0;
	}
	return false;
}

bool uipf::util::str_ends_with(const std::string& s, const std::string& with) {
	if (s.length() >= with.length()) {
		return s.compare(s.length() - with.length(), with.length(), with) == 0;
	}
	return false;
}

std::string uipf::util::str_to_upper(const std::string& st) {
	std::string s(st);
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}

std::string uipf::util::str_to_lower(const std::string& st) {
	std::string s(st);
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}


#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

// http://stackoverflow.com/a/478960/1106908
std::string uipf::util::exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	// TODO fail on non-zero exit, also provide stderr output in that case
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != NULL)
			result += buffer.data();
	}
	return result;
}
