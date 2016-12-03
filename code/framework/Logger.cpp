#include "Logger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "GUIEventDispatcher.hpp"

namespace uipf
{

Logger* Logger::instance_ = nullptr;

Logger* Logger::instance()
{
  static Logger::Guard w;
  if( instance_ == nullptr )
    instance_ = new Logger();
  return instance_;
}

void Logger::Warn(const std::string& strMessage)
{
	std::stringstream output;

	// output in yellow using ANSI codes http://en.wikipedia.org/wiki/ANSI_escape_code
	output << "\033[1;33m[WARNING] " << strMessage << "\033[0m" << std::endl;

	print(output);

	//send signal to GUI
	GUIEventDispatcher::instance()->triggerLogEvent(WARNING,strMessage);
}

void Logger::Error(const std::string& strMessage)
{
	std::stringstream output;

	// output in red using ANSI codes http://en.wikipedia.org/wiki/ANSI_escape_code
	output << "\033[1;31m[ERROR] " << strMessage << "\033[0m" << std::endl;

	print(output);

	//send signal to GUI
	GUIEventDispatcher::instance()->triggerLogEvent(ERROR,strMessage);
}

void Logger::Info(const std::string& strMessage)
{
	std::stringstream output;
	// output in bold (more visible than normal stdout) using ANSI codes http://en.wikipedia.org/wiki/ANSI_escape_code
	output << "\033[1m[INFO] " << strMessage << "\033[0m" << std::endl;
	print(output);

	//send signal to GUI
	GUIEventDispatcher::instance()->triggerLogEvent(INFO,strMessage);
}

void Logger::Debug(const std::string& strMessage)
{
	//if CMAKE_BUILD_TYPE = Release -> NDEBUG is defined
#ifndef NDEBUG
	std::stringstream output;
	output << "[DEBUG] " << strMessage << std::endl;
	print(output);

	//send signal to GUI
	GUIEventDispatcher::instance()->triggerLogEvent(DEBUG,strMessage);
#endif
}

void Logger::print(const std::stringstream& what)
{

	std::cout << what.str();
}

} // namespace
