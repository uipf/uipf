#ifndef LIBUIPF_DATA_HPP
#define LIBUIPF_DATA_HPP

#include <string>
#include <memory>
#if WITH_OPENCV
#include <opencv2/core/core.hpp>
#endif

/*
 * This file contains the Declaration for the UIPF Type system.
 *
 *
 */


// reference counting smart pointer
// this is the core for allowing passing data from one module to another
#define UIPF_SMARTPOINTER std::shared_ptr

namespace uipf {

	template <typename T>
	class Data {

	public:
		typedef UIPF_SMARTPOINTER <Data> ptr;
		typedef const UIPF_SMARTPOINTER <Data> c_ptr;

	public:
		// constructor (can't be virtual!)
		Data(void) {};

		// destructor
		virtual ~Data(void) {};

		// returns the data type of this data object
		// this is a virtual method, which has to be overwritten in the class, which derives of Data
		virtual std::string getType() const = 0;

		// returns the value of the integer
		virtual T getContent() const = 0;

		// TODO serialization
//		virtual std::string serialize() const = 0;
//		virtual ptr unserialize(std::string) const = 0;

		// TODO partitioning


	};
}

#define UIPF_BEGIN_DATA_TYPE(NAME, ID, TYPE) class NAME : public uipf::Data<TYPE> { \
	public: \
		typedef UIPF_SMARTPOINTER <NAME> ptr; \
		typedef const UIPF_SMARTPOINTER <NAME> c_ptr; \
		std::string getType() const override { return ID; }; \
		NAME(TYPE d) : data_(d) {};/* TODO constructor should not copy original data */  \
/*		NAME(const NAME& d) : data_(d.data_) {}; */ \
		~NAME(void){}; \
		TYPE getContent() const { return data_; }; \
		void setContent(TYPE d) { data_ = d; }; \
	private: \
		TYPE data_;
#define UIPF_END_DATA_TYPE };

namespace uipf {

	// define default types that ship with UIPF

	UIPF_BEGIN_DATA_TYPE (String, "de.tu-berlin.cvrs.uipf.String", std::string)
	UIPF_END_DATA_TYPE

	UIPF_BEGIN_DATA_TYPE (Bool, "de.tu-berlin.cvrs.uipf.Bool", bool)
	UIPF_END_DATA_TYPE

	UIPF_BEGIN_DATA_TYPE (Int, "de.tu-berlin.cvrs.uipf.Int", int)
	UIPF_END_DATA_TYPE

	UIPF_BEGIN_DATA_TYPE (StringList, "de.tu-berlin.cvrs.uipf.StringList", std::list<std::string>)
	UIPF_END_DATA_TYPE

	UIPF_BEGIN_DATA_TYPE (IntList, "de.tu-berlin.cvrs.uipf.StringList", std::list<int>)
	UIPF_END_DATA_TYPE


#ifdef WITH_OPENCV

	// create types for OpenCV if built with OpenCV support

	UIPF_BEGIN_DATA_TYPE (OpenCVMat, "de.tu-berlin.cvrs.opencv.Mat", cv::Mat)
	UIPF_END_DATA_TYPE


#endif

};


#endif //LIBUIPF_DATA_HPP
