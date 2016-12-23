#ifndef LIBUIPF_LOGGING_HPP
#define LIBUIPF_LOGGING_HPP

#include <iostream>

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

	}
}

// shortcuts to log
// can be used like UIPF_LOG_ERROR("something bad happened!")
// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros
// output colors using ANSI codes http://en.wikipedia.org/wiki/ANSI_escape_code
#define UIPF_LOG_ERROR(...)   uipf::log::logconcat( "\033[1;31mError: \033[0m", __VA_ARGS__ )
#define UIPF_LOG_INFO(...)    uipf::log::logconcat( "\033[1mInfo: \033[0m", __VA_ARGS__ )
#define UIPF_LOG_WARNING(...) uipf::log::logconcat( "\033[1;33mWarning: \033[0m", __VA_ARGS__ )
#define UIPF_LOG_DEBUG(...)   uipf::log::logconcat( "\033[1;36mDebug: \033[0m", __VA_ARGS__ )
#define UIPF_LOG_TRACE(...)   uipf::log::logconcat( "Trace: ", __VA_ARGS__ , "\n    - ", __FILE__, ":", __LINE__ )

#endif // LIBUIPF_LOGGING_HPP
