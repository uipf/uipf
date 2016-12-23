#ifndef LIBUIPF_LOGGING_HPP
#define LIBUIPF_LOGGING_HPP

#include <iostream>

//shortcuts to log
//can be used like LOG_E("something bad happened!")
#define UIPF_LOG_ERROR(message) std::cout << "Error: " << message << std::endl
#define UIPF_LOG_INFO(message) std::cout << "Info: " << message << std::endl
#define UIPF_LOG_WARNING(message) std::cout << "Warning: " << message << std::endl
#define UIPF_LOG_DEBUG(message) std::cout << "Debug: " << message << std::endl

#endif // LIBUIPF_LOGGING_HPP
