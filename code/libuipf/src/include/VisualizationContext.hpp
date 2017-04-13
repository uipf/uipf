#ifndef LIBUIPF_VISUALIZATIONCONTEXT_HPP
#define LIBUIPF_VISUALIZATIONCONTEXT_HPP

#include <string>
#include <opencv2/core/core.hpp>

namespace uipf {

	class VisualizationContext {
	public:

		virtual void displayImage(cv::Mat) = 0;

		virtual void displayText(std::string) = 0;

	};

};

#endif //LIBUIPF_VISUALIZATIONCONTEXT_HPP
