#include "util.hpp"

#include <vector>
#include <algorithm>


// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
/*
s	old name, which has to be renamed
*/
std::string uipf::util::rename(const std::string& s){
	return rename_postfix(s, "_result");
}

std::string uipf::util::rename_postfix(const std::string& s, const std::string& pf) {

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
		tokens.push_back(end + pf);
	} else {
		std::string end = tokens[tokens.size() - 1];
		tokens.pop_back();
		std::string beforeEnd = tokens[tokens.size() - 1];
		tokens.pop_back();
		beforeEnd = beforeEnd + pf;
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
#include <exceptions.hpp>

// http://stackoverflow.com/a/478960/1106908
std::string uipf::util::exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;

	FILE* pipe = popen(cmd, "r");
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	try {
		while (!feof(pipe)) {
			if (fgets(buffer.data(), 128, pipe) != NULL)
				result += buffer.data();
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	int ret = pclose(pipe);
	if (ret != 0) {
		throw new ErrorException(std::string("Command failed with exit code ") + std::to_string(ret) + std::string(" ") + cmd);
	}
	return result;}
