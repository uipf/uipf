#include "GuiLogger.hpp"

using namespace uipf;

GuiLogger* GuiLogger::instance_ = nullptr;

GuiLogger* GuiLogger::instance()
{
	static GuiLogger::Guard w;
	if( instance_ == nullptr )
		instance_ = new GuiLogger();
	return instance_;
}

GuiLogger::GuiLogger()
{
	qRegisterMetaType<uipf::log::Logger::LogLevel >("log::Logger::LogLevel");
	qRegisterMetaType<std::string>("std::string");

	logger_ = new GuiLoggerCallback();
	UIPF_REGISTER_LOGGER(logger_);
}

GuiLogger::~GuiLogger() {

	UIPF_UNREGISTER_LOGGER(logger_);
	delete logger_;
	UIPF_LOG_DEBUG("ende");
	UIPF_LOG_DEBUG(uipf::log::Logger::instance()->logCallbacks.size());
}

void GuiLoggerCallback::log(log::Logger::LogLevel lvl, const std::string& msg)
{
	GuiLogger::instance()->log(lvl, msg);
}
