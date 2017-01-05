#ifndef LIBUIPF_DATA_LIST_HPP
#define LIBUIPF_DATA_LIST_HPP

#include <string>
#include <vector>
#include <map>

#include "../data.hpp"

namespace uipf {
	namespace data {


		UIPF_DATA_TYPE_BEGIN (List, "de.tu-berlin.uipf.list", std::vector<Data::ptr>)
			// TODO get subtype? for typechecking
		UIPF_DATA_TYPE_END

	};
};

#endif // LIBUIPF_DATA_LIST_HPP
