#ifndef UIPFRAMEWORK_GUILOGGER_HPP
#define UIPFRAMEWORK_GUILOGGER_HPP

#include <QObject>

#include "logging.hpp"

namespace uipf {

	class GuiLoggerCallback : public log::Logger::LogCallback {

		virtual void log(log::Logger::LogLevel, const std::string&);

	};


	class GuiLogger : public QObject {
		Q_OBJECT

	public:
		static GuiLogger* instance();

	private:
		// holds the singleton instance of the GUIEventDispatcher
		static GuiLogger *instance_;
		GuiLogger();
		GuiLogger( const GuiLogger& );
		~GuiLogger();

		// helper class to ensure GUIEventDispatcher gets deleted when the context is gone
		class Guard {
		public: ~Guard() {
				if( GuiLogger::instance_ != 0 ) {
					delete GuiLogger::instance_;
				}
			}
		};
		friend class Guard;

		log::Logger::LogCallback* logger_;

	public:

		void log(log::Logger::LogLevel, const std::string &);

	signals: //for QT to connect
		void logEvent(log::Logger::LogLevel lvl, const std::string &strMessage);

	};

}

#endif //UIPFRAMEWORK_GUILOGGER_HPP
