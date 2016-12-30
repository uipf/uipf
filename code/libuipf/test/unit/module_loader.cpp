#include "ModuleLoader.hpp"

// - TODO copy modules to different paths and make sure they get found
// - TODO load module, check if loading works,
// - TODO load module, check meta data

#include <vector>
#include <string>
#include <map>
#include "util.hpp"


#define BOOST_TEST_MODULE libuipf-module_loader
#include <boost/test/included/unit_test.hpp>

inline std::vector<std::string> to_vector(std::map<std::string,std::string> m) {
	std::vector<std::string> v;
	uipf_foreach(e, m) {
		v.push_back(e->second);
	}
	return v;
}

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
	// TODO test ids
	vector<string> names = to_vector(loader.getModuleNames());
	BOOST_CHECK_EQUAL_COLLECTIONS(names.begin(), names.end(), expectedNames.begin(), expectedNames.end());

	// TODO test getModuleCategories

	BOOST_TEST(loader.hasModule("consumer"));
	BOOST_TEST(loader.hasModule("producer"));
	BOOST_TEST(!loader.hasModule("other"));
	BOOST_TEST(!loader.hasModule("other"));

	loader.addSearchPath("./test/integration/compiler-gcc");

	// should have loaded one additional module
	BOOST_CHECK_EQUAL(3, loader.getModuleNames().size());
	vector<string> names2 = to_vector(loader.getModuleNames());
	if (std::find(names2.begin(), names2.end(), "GccModule") == names2.end()
	 ||	std::find(names2.begin(), names2.end(), "producer") == names2.end()
	 ||	std::find(names2.begin(), names2.end(), "consumer") == names2.end()
	) {
		BOOST_TEST_FAIL("Module names list is incomplete.");
	}

	BOOST_TEST(loader.hasModule("consumer"));
	BOOST_TEST(loader.hasModule("producer"));
	BOOST_TEST(loader.hasModule("GccModule"));
	BOOST_TEST(!loader.hasModule("other"));

	loader.addSearchPath("./test/integration/compiler-clang/libClangModule.so");

	// should have loaded one additional module
	BOOST_CHECK_EQUAL(4, loader.getModuleNames().size());
	vector<string> names3 = to_vector(loader.getModuleNames());
	if (std::find(names3.begin(), names3.end(), "GccModule") == names3.end()
	    ||	std::find(names3.begin(), names3.end(), "ClangModule") == names3.end()
	    ||	std::find(names3.begin(), names3.end(), "producer") == names3.end()
	    ||	std::find(names3.begin(), names3.end(), "consumer") == names3.end()
			) {
		BOOST_TEST_FAIL("Module names list is incomplete.");
	}

	BOOST_TEST(loader.hasModule("consumer"));
	BOOST_TEST(loader.hasModule("producer"));
	BOOST_TEST(loader.hasModule("GccModule"));
	BOOST_TEST(loader.hasModule("ClangModule"));
	BOOST_TEST(!loader.hasModule("other"));

}