#ifndef UIPFRAMEWORK_GUIVISUALIZATIONCONTEXT_HPP
#define UIPFRAMEWORK_GUIVISUALIZATIONCONTEXT_HPP

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>

#include "VisualizationContext.hpp"

namespace uipf {

class MainWindow;

class GuiVisualizationContext : public QObject, public VisualizationContext
{
	Q_OBJECT

public:
	virtual void displayImage(cv::Mat);
	virtual void displayText(std::string);

signals: //for QT to connect
	void createImageWindow(const std::string& strTitle);
	void createTextWindow(const std::string& strTitle, const std::string& text);

private:
	// used by triggerCreateWindow to transfer the image into the GUI thread
	QImage image_;

	// protected properties accessable by the MainWindow, used for displaying image windows
	friend class MainWindow;

	// mutex and condition to communicate with the GUI thread to figure out when the image was
	// rendered and memory can be freed by our thread.
	QMutex mutex;
	QWaitCondition imageRendered;



};

}


#endif //UIPFRAMEWORK_GUIVISUALIZATIONCONTEXT_HPP
