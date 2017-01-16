#ifdef WITH_OPENCV

#include "data/opencv.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

#include "util.hpp"

using namespace cv;
using namespace uipf;
using namespace uipf::data;

OpenCVMat::ptr uipf::data::load_image_color(const std::string& filename) {

	Mat image;
	image = imread(filename.c_str(), CV_LOAD_IMAGE_COLOR);

	if (!image.data) { // Check for invalid input
		throw ErrorException(string("Could not open or find the image: ") + filename);
	}
	OpenCVMat::ptr mat(new OpenCVMat(image));
	mat->filename = filename;

	return mat;
}

OpenCVMat::ptr uipf::data::load_image_greyscale(const std::string& filename) {

	Mat image;
	image = imread(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

	if (!image.data) { // Check for invalid input
		throw ErrorException(string("Could not open or find the image: ") + filename);
	}
	OpenCVMat::ptr mat(new OpenCVMat(image));
	mat->filename = filename;

	return mat;
}

List::ptr uipf::data::load_images_color(const std::string& path) {

	std::vector<std::string> names = load_image_names(path);
	List::ptr list(new List());
	uipf_foreach(name, names) {
		list->getContent().push_back(load_image_color(*name));
	}
	return list;
}

uipf::data::List::ptr uipf::data::load_images_greyscale(const std::string& path) {

	std::vector<std::string> names = load_image_names(path);
	List::ptr list(new List());
	uipf_foreach(name, names) {
		list->getContent().push_back(load_image_greyscale(*name));
	}
	return list;
}

std::vector<std::string> uipf::data::load_image_names(const std::string& path) {

	namespace fs = boost::filesystem;

	fs::path p(path);
//	if (p.is_relative()) {
//		p = system_complete(p);
//	}

	std::vector<std::string> list;

	try {
		if (!fs::exists(p)) {
			throw ErrorException(std::string("Search path does not exist: ") + p.string());
		} else if (fs::is_directory(p)) {

			fs::directory_iterator end_itr;
			for (fs::directory_iterator itr( p ); itr != end_itr; ++itr) {
				// try to load library if it is a file ending with system specific name, .so/.dll
				if (fs::is_regular_file(itr->path())) {
					list.push_back(itr->path().string());
				}
			}

		} else if (fs::is_regular_file(p)) {
			list.push_back(p.string());
		} else {
			throw ErrorException(std::string("Search path is not a regular file or directory: ") + p.string());
		}
	}
	catch (const fs::filesystem_error& ex) {
		throw ErrorException(std::string("Failed to read search path: ") + p.string() + std::string(" : ") + ex.what());
	}
	return list;
}

bool uipf::data::OpenCVMat::getIsStored() const {
	return !filename.empty();
}

void uipf::data::OpenCVMat::store(const std::string& f, const std::vector<int>& params) {
	cv::imwrite(f, getContent(), params);
	filename = f;
}


#endif
