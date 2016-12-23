#include "ModuleLoader.hpp"

// - TODO copy modules to different paths and make sure they get found
// - TODO load module, check if loading works,
// - TODO load module, check meta data


#define BOOST_TEST_MODULE libuipf-module_loader
#include <boost/test/included/unit_test.hpp>
#include <opencv2/core/core.hpp>

BOOST_AUTO_TEST_CASE(ModuleLoaderLoadModules)
{
	using namespace uipf;

	ModuleLoader loader;

	loader.addSearchPath("./test/integration/dataflow");

	// path contains two modules
	BOOST_CHECK_EQUAL(2, loader.getModuleNames().size());

	BOOST_TEST(loader.hasModule("consumer"));
	BOOST_TEST(loader.hasModule("producer"));
	BOOST_TEST(!loader.hasModule("other"));


}