#ifndef LIBUIPF_LOGGING_HPP
#define LIBUIPF_LOGGING_HPP

#include <iostream>
#include <sstream>
#include <vector>


namespace uipf {
	namespace log {

		// a varadic concat for logging
		// http://en.cppreference.com/w/cpp/utility/variadic
		// http://stackoverflow.com/questions/2770474/how-to-find-the-length-of-a-parameter-pack
		template<typename T>
		std::string logconcat(T value) {
			std::ostringstream s;
			s << value;
			return s.str();
		}
		template<typename T, typename... Targs>
		std::string logconcat(T value, Targs... Fargs) {

			std::size_t argc = sizeof...(Targs);

			std::ostringstream s;
			s << value;

			if (argc > 0) {
				return s.str() + logconcat(Fargs...);
			}
			return s.str();
		}

/*		template<typename T>
		std::string tracefilename(const std::string& file) {
			boost::filesystem::path p(file);
			return p.filename().string();
		}*/



		//This is a simple Logger implemented as singleton, that writes to console and sends messages to the GUI.
		//Herefore it uses the GUIEventDispatcher.
		//Messages are logged threadsafe. So it does not matter if e.g. LOG_I() is called in a GUI-Thread or from a Backgroundworker.
		class Logger
		{
			public:
				enum LogLevel
				{
					NONE = 0,
					ERROR = 1,
					WARNING = 2,
					INFO = 3,
					DEBUG = 4,
					TRACE = 5,
				};
			public:
				static Logger* instance();

			private:
				// holds the singleton instance of the logger
				static Logger *instance_;
				Logger() {}
				Logger( const Logger& );
				~Logger() {}

				// helper class to ensure logger gets deleted when the context is gone
				class Guard {
				public: ~Guard() {
						if( Logger::instance_ != 0 ) {
							delete Logger::instance_;
						}
					}
				};
				friend class Guard;

			public:
				//Warnings are colored blue in the GUI
				void Warn(const std::string& strMessage);
				//Errors are colored red in the GUI
				void Error(const std::string& strMessage);
				//Info are colored green in the GUI
				void Info(const std::string& strMessage);
				//Debugmessages can be used internally. They are shown when the project is built with CMAKE_BUILD_TYPE=Debug
				void Debug(const std::string& strMessage);
				void Trace(const std::string& strMessage);

				LogLevel level = INFO;

				class LogCallback
				{
					public:
						virtual ~LogCallback() {};
						virtual void log(LogLevel, const std::string&) = 0;
				};

				std::vector<LogCallback*> logCallbacks;

			private:

				void print(const std::stringstream& what);

		};

	}
}

#define UIPF_LOG_LEVEL uipf::log::Logger::instance()->level
#define UIPF_REGISTER_LOGGER(fun) uipf::log::Logger::instance()->logCallbacks.push_back(fun);
#define UIPF_UNREGISTER_LOGGER(fun) for(auto it = uipf::log::Logger::instance()->logCallbacks.begin(); it != uipf::log::Logger::instance()->logCallbacks.end(); it++) { if (*it == fun) { uipf::log::Logger::instance()->logCallbacks.erase(it); break; }}

// shortcuts to log
// can be used like UIPF_LOG_ERROR("something bad happened!")
// https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros
// output colors using ANSI codes http://en.wikipedia.org/wiki/ANSI_escape_code
#define UIPF_LOG_ERROR(...)     uipf::log::Logger::instance()->Error( uipf::log::logconcat( __VA_ARGS__ ) );
#define UIPF_LOG_WARNING(...)   uipf::log::Logger::instance()->Warn(  uipf::log::logconcat( __VA_ARGS__ ) );
#define UIPF_LOG_INFO(...)      uipf::log::Logger::instance()->Info(  uipf::log::logconcat( __VA_ARGS__ ) );
#define UIPF_LOG_DEBUG(...)     uipf::log::Logger::instance()->Debug( uipf::log::logconcat( __VA_ARGS__ ) );
#ifndef NDEBUG
	#define UIPF_LOG_TRACE(...) uipf::log::Logger::instance()->Trace( uipf::log::logconcat( /*uipf::log::tracefilename<std::string>(*/__FILE__, ":", __LINE__, ": ", __VA_ARGS__) );
#else
	#define UIPF_LOG_TRACE(...)
#endif

#endif // LIBUIPF_LOGGING_HPP
