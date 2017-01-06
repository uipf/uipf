#ifndef LIBUIPF_DATA_OPENCV_HPP
#define LIBUIPF_DATA_OPENCV_HPP

#include <opencv2/core/core.hpp>

#include "../data.hpp"

namespace uipf {
	namespace data {

		// create types for OpenCV if built with OpenCV support

		UIPF_DATA_TYPE_BEGIN (OpenCVMat, "de.tu-berlin.opencv.Mat", cv::Mat)

		public:

			/**
			 * The file name of the original image file.
			 */
			std::string filename;

		UIPF_DATA_TYPE_END

	};
};

#endif // LIBUIPF_DATA_OPENCV_HPP
