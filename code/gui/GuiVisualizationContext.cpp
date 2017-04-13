#include <opencv2/imgproc/imgproc.hpp>
#include "data/opencv.hpp"
#include "GuiVisualizationContext.hpp"
#include "logging.hpp"

using namespace uipf;

void GuiVisualizationContext::displayImage(cv::Mat image)
{
	using namespace cv;

	//this code is inspired by opencv source /modules/highgui/src/window_QT.cpp
	//we create a standardised tmp image to support different image types (rgb,grayscale)
	//other types need to be tested...

	Mat tmp = Mat(image.rows, image.cols, CV_8UC3);

	if (image.channels() == 3) {
		// assume RGB image for 3 channels
		cvtColor(image, tmp, CV_BGR2RGB);
	} else if (image.channels() == 1) {
		// assume Grayscale image for 1 channel
		// convert gray to RGB since gray image display in QImage is only supported since Qt 5.4
		cvtColor(image, tmp, CV_GRAY2RGB);
	} else {
		UIPF_LOG_ERROR("Unsupported number of channels for displaying image.");
		mutex.unlock();
		return;
	}

	image_ = QImage(tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);

	// send signal to GUI
	emit createImageWindow(string("Image"));
}

void GuiVisualizationContext::displayText(std::string text)
{

	emit createTextWindow(std::string("Text"), text);
}
