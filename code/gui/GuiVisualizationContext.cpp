#include <opencv2/imgproc/imgproc.hpp>
#include "data/opencv.hpp"
#include "GuiVisualizationContext.hpp"
#include "logging.hpp"

using namespace uipf;

void GuiVisualizationContext::displayImage(cv::Mat image)
{
	using namespace cv;

	UIPF_LOG_TRACE("displayImage() called");

	// lock needed to ensure only one image is displayed at a time to avoid different threads to interfere
	mutex.lock();
	UIPF_LOG_TRACE("displayImage -> lock()");

	//this code is inspired by opencv source /modules/highgui/src/window_QT.cpp
	//we create a standardised tmp image to support different image types (rgb,grayscale)
	//other types need to be tested...

	// when changing this code make sure tmp is in scope until the mutex is unlocked.
	// This is because it holds the data of the image that is displayed in the GUI thread.
	// If tmp falls out of scope before the image is rendered in the GUI it will delete its
	// memory because of the OpenCV internal smartpointer implementation and the image display
	// will look corrupt.
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

	UIPF_LOG_TRACE("displayImage -> signal");

	// send signal to GUI
	emit createWindow(string("Image"));

	UIPF_LOG_TRACE("displayImage -> waiting...");

	// wait for the GUI to render the image so that resources of this thread are not freed before GUI can use them
//	imageRendered.wait(&mutex);

	UIPF_LOG_TRACE("displayImage -> unlock()");

	// unlock the mutex created above
	mutex.unlock();
}
