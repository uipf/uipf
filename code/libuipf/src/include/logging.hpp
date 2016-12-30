#ifndef LIBUIPF_LOGGING_HPP
#define LIBUIPF_LOGGING_HPP

#include <iostream>
// TODO skip this external dependency introduce logging.cpp
#include <boost/filesystem.hpp>

namespace uipf {
	namespace log {

		// a varadic concat for logging
		// http://en.cppreference.com/w/cpp/utility/variadic
		// http://stackoverflow.com/questions/2770474/how-to-find-the-length-of-a-parameter-pack
		template<typename T>
		void logconcat(T value) {
			std::cout << value << std::endl;
		}
		template<typename T, typename... Targs>
		void logconcat(T value, Targs... Fargs) {

			std::size_t argc = sizeof...(Targs);

			std::cout << value;

			if (argc > 0) {
				logconcat(Fargs...);
			}
		}

		template<typename T>
		std::string tracefilename(const std::string& file) {
			boost::filesystem::path p(file);
			return p.filename().string();
		}

	}
}

// you may define UIPF_LOG_LEVEL in your code to an expression that is used to determine the logging level
#ifndef UIPF_LOG_LEVEL
#define UIPF_LOG_LEVEL 3
#endif

// shortcuts to log
// can be used like UIPF_LOG_ERROR("something bad happened!")
// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros
// output colors using ANSI codes http://en.wikipedia.org/wiki/ANSI_escape_code
#define UIPF_LOG_ERROR(...)   if (UIPF_LOG_LEVEL >= 1) { uipf::log::logconcat( "\033[1;31mError: \033[0m", __VA_ARGS__ ); }
#define UIPF_LOG_WARNING(...) if (UIPF_LOG_LEVEL >= 2) { uipf::log::logconcat( "\033[1;33mWarning: \033[0m", __VA_ARGS__ ); }
#define UIPF_LOG_INFO(...)    if (UIPF_LOG_LEVEL >= 3) { uipf::log::logconcat( "\033[1mInfo: \033[0m", __VA_ARGS__ ); }
#define UIPF_LOG_DEBUG(...)   if (UIPF_LOG_LEVEL >= 4) { uipf::log::logconcat( "\033[1;36mDebug: \033[0m", __VA_ARGS__ ); }
#define UIPF_LOG_TRACE(...)   if (UIPF_LOG_LEVEL >= 5) { uipf::log::logconcat( "\033[1mTrace ", uipf::log::tracefilename<std::string>(__FILE__), ":", __LINE__, ":\033[0m ", __VA_ARGS__); }

#endif // LIBUIPF_LOGGING_HPP
