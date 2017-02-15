#ifndef LIBUIPF_DATA_LIST_HPP
#define LIBUIPF_DATA_LIST_HPP

#include <string>
#include <vector>
#include <map>

#include "../data.hpp"

namespace uipf {
	namespace data {


		UIPF_DATA_TYPE_BEGIN (Map, "map", std::map<std::string, Data::ptr>)

			// TODO getType() in combination


			// TODO get subtype? for typechecking
		UIPF_DATA_TYPE_END

	};
};

#endif // LIBUIPF_DATA_LIST_HPP
