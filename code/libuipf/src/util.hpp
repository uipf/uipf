#ifndef LIBUIPF_UTIL_HPP
#define LIBUIPF_UTIL_HPP

#include <string>
#include <sstream>

// a macro to simply simple for loops
#define uipf_foreach(i, set) for(auto i = set.cbegin(); i != set.end(); ++i)


// This is a collection of help-methods

namespace uipf {
	namespace util {

		// TODO refactor these to "split()"
		// gets the first part of the string, which is divided by : , e.g. source:../ball.png -> source
		std::string firstPart(std::string s);
		// gets the second part of the string, which is divided by : , e.g. source:../ball.png -> ../ball.png
		std::string secondPart(std::string s);

		// renames a filename by adding _result before the end, eg. ball.png -> ball_result.png
		std::string rename(std::string s);

	}
}

#endif //LIBUIPF_UTIL_HPP
