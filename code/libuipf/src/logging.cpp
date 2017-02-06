#include "logging.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace uipf::log;

Logger* Logger::instance_ = nullptr;

Logger* Logger::instance()
{
	static Logger::Guard w;
	if (instance_ == nullptr) {
	  instance_ = new Logger();
	}
	return instance_;
}

void Logger::Error(const std::string& strMessage)
{
	if (level < ERROR) {
		return;
	}
	for (LogFunction f: logCallbacks) {
		f(ERROR, strMessage);
	}
}

void Logger::Warn(const std::string& strMessage)
{
	if (level < WARNING) {
		return;
	}
	for (LogFunction f: logCallbacks) {
		f(WARNING, strMessage);
	}
}

void Logger::Info(const std::string& strMessage)
{
	if (level < INFO) {
		return;
	}
	for (LogFunction f: logCallbacks) {
		f(INFO, strMessage);
	}
}
void Logger::Debug(const std::string& strMessage)
{
	if (level < DEBUG) {
		return;
	}
	for (LogFunction f: logCallbacks) {
		f(DEBUG, strMessage);
	}
}
void Logger::Trace(const std::string& strMessage)
{
	if (level < TRACE) {
		return;
	}
	for (LogFunction f: logCallbacks) {
		f(TRACE, strMessage);
	}
}
