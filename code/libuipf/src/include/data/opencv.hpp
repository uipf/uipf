#ifndef LIBUIPF_DATA_OPENCV_HPP
#define LIBUIPF_DATA_OPENCV_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <map>
#include <string>

#include "../data.hpp"
#include "list.hpp"

namespace uipf {
	namespace data {

		// create types for OpenCV if built with OpenCV support

		UIPF_DATA_TYPE_BEGIN (OpenCVMat, "uipf.opencv.Mat", cv::Mat)

		public:

			/**
			 * The file name of the original image file.
			 */
			std::string filename;

			/**
			 * EXIF meta data, if any was found in the file.
			 */
			std::map<std::string, std::string> exif;

			/**
			 * @return whether this images is stored on the disk. If true, filename is set.
			 */
			bool getIsStored() const;

			// TODO store does not store EXIF data right now
			void store(const std::string& f, const std::vector<int>& params=std::vector<int>());

			std::vector<std::string> visualizations() const;

			void visualize(std::string option, VisualizationContext& context) const;


		UIPF_DATA_TYPE_END

		OpenCVMat::ptr load_image_color(const std::string& filename);
		OpenCVMat::ptr load_image_greyscale(const std::string& filename);
		std::map<std::string, std::string> load_image_exif_data(const std::string& filename);

		uipf::data::List::ptr load_images_color(const std::string& path);
		uipf::data::List::ptr load_images_greyscale(const std::string& path);
		std::vector<std::string> load_image_names(const std::string& path);

	};
};

#endif // LIBUIPF_DATA_OPENCV_HPP
