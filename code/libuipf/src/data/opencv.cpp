#ifdef WITH_OPENCV

#include "VisualizationContext.hpp"
#include "data/opencv.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <map>

#include "logging.hpp"
#include "util.hpp"

using namespace std;
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

	mat->exif = load_image_exif_data(filename);

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

	mat->exif = load_image_exif_data(filename);

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

std::map<std::string, std::string> uipf::data::load_image_exif_data(const std::string& filename)
{
	std::map<std::string, std::string> exif;

	string output;
	try {
		output = uipf_exec_stdout((string("jhead \"") + filename + string("\"")).c_str());
	} catch(uipf::ErrorException& e) {
		UIPF_LOG_WARNING("Failed to load EXIF information for image ", filename, ": ", e.what());
		return exif;
	}
	// output should contain something like:
//	File name    : kermit000.jpg
//	File size    : 165140 bytes
//	File date    : 2017:01:14 07:39:31
//	Camera make  : Canon
//	Camera model : Canon PowerShot A10
//	Date/Time    : 2004:11:03 21:10:01
//	Resolution   : 640 x 480
//	Flash used   : No
//	Focal length :  5.4mm  (35mm equivalent: 37mm)
//	CCD width    : 5.23mm
//	Exposure time: 0.050 s  (1/20)
//	Aperture     : f/2.8
//	Focus dist.  : 0.62m
//	Light Source : Fluorescent
//	Metering Mode: pattern
//	JPEG Quality : 97

	for(string line: split_string(output, '\n')) {
		if (line.empty()) {
			continue;
		}
		vector<string> parts = split_string(line, ':');
		string name = parts[0];
		trim(name);
		if (name.empty()) {
			continue;
		}
		string value;
		for(size_t i = 1; i < parts.size(); ++i) {
			value += parts[i];
		}
		exif.insert(pair<string, string>(name, trim(value)));
	}
	return exif;
}

bool uipf::data::OpenCVMat::getIsStored() const {
	return !filename.empty();
}

void uipf::data::OpenCVMat::store(const std::string& f, const std::vector<int>& params) {
	cv::imwrite(f, getContent(), params);
	filename = f;
}

void uipf::data::OpenCVMat::visualize(std::string option, VisualizationContext& context) const {
	if (option.compare("Image") == 0) {
		UIPF_LOG_TRACE("showing opencv mat");
		context.displayImage(getContent());
		return;
	}
	if (option.compare("Name") == 0) {
		UIPF_LOG_TRACE("showing filename");
		context.displayText(filename);
		return;
	}
	if (option.compare("EXIF") == 0) {
		std::ostringstream s;
		for(auto exifItem: exif) {
			s << exifItem.first << " : " << exifItem.second << "\n";
		}
		context.displayText(s.str());
		return;
	}
}

std::vector<std::string> OpenCVMat::visualizations() const {
	std::vector<std::string> options = {
		"Image",
	    "Name"
	};
	if (!exif.empty()) {
		options.push_back("EXIF");
	}
	return options;
}


#endif
