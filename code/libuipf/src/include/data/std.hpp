#ifndef LIBUIPF_DATA_STD_HPP
#define LIBUIPF_DATA_STD_HPP

#include <string>
#include <vector>
#include <map>

#include "../data.hpp"

namespace uipf {
	namespace data {

		// define default types that ship with UIPF

		UIPF_BEGIN_DATA_TYPE (String, "de.tu-berlin.uipf.String", std::string)
		UIPF_END_DATA_TYPE

		UIPF_BEGIN_DATA_TYPE (Bool, "de.tu-berlin.uipf.Bool", bool)
		UIPF_END_DATA_TYPE

		UIPF_BEGIN_DATA_TYPE (Int, "de.tu-berlin.uipf.Int", int)
		UIPF_END_DATA_TYPE

		UIPF_BEGIN_DATA_TYPE (StringList, "de.tu-berlin.uipf.StringList", std::vector<std::string>)
		UIPF_END_DATA_TYPE

		UIPF_BEGIN_DATA_TYPE (IntList, "de.tu-berlin.uipf.StringList", std::vector<int>)
		UIPF_END_DATA_TYPE

	};
};

#endif // LIBUIPF_DATA_STD_HPP
