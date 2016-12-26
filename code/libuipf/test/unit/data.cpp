#include "Data.hpp"

#define BOOST_TEST_MODULE libuipf-data
#include <boost/test/included/unit_test.hpp>
#include <opencv2/core/core.hpp>

BOOST_AUTO_TEST_CASE(Data_Int)
{
	using namespace uipf;
	Int i1(42);
	BOOST_TEST( i1.getContent() == 42 );

	int ival = 64;
	Int i2(ival);
	BOOST_TEST( i2.getContent() == ival );
	BOOST_TEST( i2.getContent() == 64 );
	BOOST_TEST( ival == 64 );

	BOOST_CHECK_EQUAL( i1.getType() , "de.tu-berlin.uipf.Int" );
}

// TODO test Bool and String

// TODO test serialization

BOOST_AUTO_TEST_CASE(Data_IntList)
{
	using namespace uipf;

	std::vector<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	IntList l(list);
	BOOST_TEST( l.getContent().size() == 3 );

//	int ival = 64;
//	Int i2(ival);
//	BOOST_TEST( i2.getContent() == ival );
//	BOOST_TEST( i2.getContent() == 64 );
//	BOOST_TEST( ival == 64 );
//
//	BOOST_CHECK_EQUAL( i1.getType() , "de.tu-berlin.uipf.Int" );
}


#ifdef WITH_OPENCV

BOOST_AUTO_TEST_CASE(Data_opencv_Mat)
{
	using namespace uipf;
	cv::Mat m(2, 2, CV_32S);
	m.at<int>(0,0) = 1;
	m.at<int>(1,0) = 2;
	m.at<int>(0,1) = 3;
	m.at<int>(1,1) = 4;
	cv::Mat* mp = &m;
	OpenCVMat mc(m);

	const cv::Mat& content = mc.getContent();

	BOOST_CHECK_EQUAL( content.rows , 2 );
	BOOST_CHECK_EQUAL( content.cols , 2 );
	BOOST_CHECK_EQUAL( content.at<int>(0,0) , 1 );
	BOOST_CHECK_EQUAL( content.at<int>(1,0) , 2 );
	BOOST_CHECK_EQUAL( content.at<int>(0,1) , 3 );
	BOOST_CHECK_EQUAL( content.at<int>(1,1) , 4 );

	BOOST_CHECK_EQUAL( &content , mp );

	BOOST_CHECK_EQUAL( mc.getType() , "de.tu-berlin.opencv.Mat" );

}

#endif

// TODO test:
// data created by one module but consumed by two should be not overwritten, but do not copy too often