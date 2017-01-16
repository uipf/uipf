#ifndef LIBUIPF_UTIL_HPP
#define LIBUIPF_UTIL_HPP

#include <string>
#include <sstream>

// a macro to simply simple for loops
#define uipf_cforeach(i, set) for(auto i = set.cbegin(); i != set.end(); ++i)
#define uipf_foreach(i, set) for(auto i = set.begin(); i != set.end(); ++i)


// This is a collection of help-methods

namespace uipf {
	namespace util {

		// TODO refactor these to "split()"
		// gets the first part of the string, which is divided by : , e.g. source:../ball.png -> source
		std::string firstPart(const std::string& s);
		// gets the second part of the string, which is divided by : , e.g. source:../ball.png -> ../ball.png
		std::string secondPart(const std::string& s);

		// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
		std::string rename(const std::string& s);

		bool str_begins_with(const std::string& s, const std::string& with);
		bool str_ends_with(const std::string& s, const std::string& with);

		std::string str_to_lower(const std::string& st);
		std::string str_to_upper(const std::string& st);

		// execute external program and return stdout
		std::string exec(const char* cmd);


	}
}

#endif //LIBUIPF_UTIL_HPP
