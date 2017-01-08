#ifndef LIBUIPF_DATA_OPENCV_HPP
#define LIBUIPF_DATA_OPENCV_HPP

#include <opencv2/core/core.hpp>

#include "../data.hpp"
#include "list.hpp"

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

		OpenCVMat::ptr load_image_color(const std::string& filename);
		OpenCVMat::ptr load_image_greyscale(const std::string& filename);

		uipf::data::List::ptr load_images_color(const std::string& path);
		uipf::data::List::ptr load_images_greyscale(const std::string& path);
		std::vector<std::string> load_image_names(const std::string& path);

	};
};

#endif // LIBUIPF_DATA_OPENCV_HPP
