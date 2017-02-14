#include "util.hpp"

#include <vector>
#include <algorithm>


// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
/*
s	old name, which has to be renamed
*/
std::string uipf_rename(const std::string& s){
	return uipf_rename_postfix(s, "_result");
}

std::string uipf_rename_postfix(const std::string& s, const std::string& pf) {

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
std::string uipf_split_second(const std::string& s){

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
std::string uipf_split_first(const std::string &s){

	std::istringstream iss(s);

	std::vector<std::string> tokens;
	std::string token;

	while (getline(iss, token, ':')) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens[0];
}

bool uipf_str_begins_with(const std::string& s, const std::string& with) {
	if (s.length() >= with.length()) {
		return s.compare(0, with.length(), with) == 0;
	}
	return false;
}

bool uipf_str_ends_with(const std::string& s, const std::string& with) {
	if (s.length() >= with.length()) {
		return s.compare(s.length() - with.length(), with.length(), with) == 0;
	}
	return false;
}

std::string uipf_str_to_upper(const std::string& st) {
	std::string s(st);
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	return s;
}

std::string uipf_str_to_lower(const std::string& st) {
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
// TODO Replace popen and pclose with _popen and _pclose for Windows.
std::string uipf_exec_stdout(const char *cmd) {
	char buffer[128];
	std::string result;

	FILE* pipe = popen(cmd, "r");
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	try {
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	int ret = pclose(pipe);
	if (ret != 0) {
		throw new uipf::ErrorException(std::string("Command failed with exit code ") + std::to_string(ret) + std::string(" ") + cmd);
	}
	return result;
}
