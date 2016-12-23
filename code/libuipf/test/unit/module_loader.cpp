#include "ModuleLoader.hpp"

// - TODO copy modules to different paths and make sure they get found
// - TODO load module, check if loading works,
// - TODO load module, check meta data


#define BOOST_TEST_MODULE My Test
#include <boost/test/included/unit_test.hpp>
#include <opencv2/core/core.hpp>

BOOST_AUTO_TEST_CASE(LoadModules)
{
	using namespace uipf;

	ModuleLoader l();



	BOOST_TEST( i1.getContent() == 42 );

	int ival = 64;
	Int i2(ival);
	BOOST_TEST( i2.getContent() == ival );
	BOOST_TEST( i2.getContent() == 64 );
	BOOST_TEST( ival == 64 );

	BOOST_CHECK_EQUAL( i1.getType() , "de.tu-berlin.uipf.Int" );
}