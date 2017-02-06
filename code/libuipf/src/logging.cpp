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
	for (LogCallback* f: logCallbacks) {
		f->log(ERROR, strMessage);
	}
}

void Logger::Warn(const std::string& strMessage)
{
	if (level < WARNING) {
		return;
	}
	for (LogCallback* f: logCallbacks) {
		f->log(WARNING, strMessage);
	}
}

void Logger::Info(const std::string& strMessage)
{
	if (level < INFO) {
		return;
	}
	for (LogCallback* f: logCallbacks) {
		f->log(INFO, strMessage);
	}
}
void Logger::Debug(const std::string& strMessage)
{
	if (level < DEBUG) {
		return;
	}
	for (LogCallback* f: logCallbacks) {
		f->log(DEBUG, strMessage);
	}
}
void Logger::Trace(const std::string& strMessage)
{
	if (level < TRACE) {
		return;
	}
	for (LogCallback* f: logCallbacks) {
		f->log(TRACE, strMessage);
	}
}
