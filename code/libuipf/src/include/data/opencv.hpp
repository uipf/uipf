#ifndef LIBUIPF_DATA_OPENCV_HPP
#define LIBUIPF_DATA_OPENCV_HPP

#include <opencv2/core/core.hpp>

#include "../data.hpp"

namespace uipf {
	namespace data {

		// create types for OpenCV if built with OpenCV support

		UIPF_BEGIN_DATA_TYPE (OpenCVMat, "de.tu-berlin.opencv.Mat", cv::Mat)
		UIPF_END_DATA_TYPE

	};
};

#endif // LIBUIPF_DATA_OPENCV_HPP
