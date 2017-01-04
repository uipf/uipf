#include "util.hpp"

#include <vector>


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

