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

// http://stackoverflow.com/a/236803/1106908
void split_string_into(const std::string &s, char delim, std::back_insert_iterator<std::vector<std::string>> result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}
std::vector<std::string> split_string(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split_string_into(s, delim, std::back_inserter(elems));
	return elems;
}

// http://stackoverflow.com/a/29892589/1106908
std::string & ltrim(std::string & str)
{
	auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
	str.erase( str.begin() , it2);
	return str;
}
std::string & rtrim(std::string & str)
{
	auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
	str.erase( it1.base() , str.end() );
	return str;
}
std::string & trim(std::string & str)
{
	return ltrim(rtrim(str));
}
std::string trim_copy(std::string const & str)
{
	auto s = str;
	return ltrim(rtrim(s));
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
// TODO add shell args vector and implement proper escaping!
// http://stackoverflow.com/questions/5510343/escape-command-line-arguments-in-c-sharp
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
