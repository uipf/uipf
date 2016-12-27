#include "ModuleLoader.hpp"

// - TODO copy modules to different paths and make sure they get found
// - TODO load module, check if loading works,
// - TODO load module, check meta data


#define BOOST_TEST_MODULE libuipf-module_loader
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ModuleLoaderLoadModules)
{
	using namespace uipf;
	using namespace std;

	// create module loader
	ModuleLoader loader;

	// without search
	BOOST_TEST(!loader.hasModule("consumer"));
	BOOST_TEST(!loader.hasModule("producer"));
	BOOST_TEST(!loader.hasModule("other"));


	loader.addSearchPath("./test/integration/dataflow");

	// path contains two modules
	BOOST_CHECK_EQUAL(2, loader.getModuleNames().size());
	vector<string> expectedNames = {"consumer", "producer"};
	vector<string> names = loader.getModuleNames();
	BOOST_CHECK_EQUAL_COLLECTIONS(names.begin(), names.end(), expectedNames.begin(), expectedNames.end());

	// TODO test getModuleCategories

	BOOST_TEST(loader.hasModule("consumer"));
	BOOST_TEST(loader.hasModule("producer"));
	BOOST_TEST(!loader.hasModule("other"));


}