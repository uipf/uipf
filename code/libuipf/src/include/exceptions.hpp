#ifndef LIBUIPF_EXCEPTIONS_HPP
#define LIBUIPF_EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace uipf {

	//
	// represents a configuration issue or configuration error in a module execution
	//
	class InvalidConfigException : public std::exception{

	public:
		// constructor
		InvalidConfigException(void) : message_("") {};
		InvalidConfigException(std::string message) : message_(message) {};

		virtual const char* what() const noexcept { return message_.c_str(); };

	private:
		std::string message_;

	};

	//
	// represents an error in a module execution
	//
	class ErrorException : public std::exception{

	public:
		// constructor
		ErrorException(void) : message_("") {};
		ErrorException(std::string message) : message_(message) {};

		virtual const char* what() const noexcept { return message_.c_str(); };

	private:
		std::string message_;

	};


}; // namespace

#define UIPF_ASSERT(expr) \
   if (!(expr)) throw uipf::ErrorException( "Assertation failed: (" #expr ") in " __STRING(__FILE__) ":" __STRING(__LINE__));

#endif // LIBUIPF_EXCEPTIONS_HPP
