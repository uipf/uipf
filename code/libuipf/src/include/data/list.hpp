#ifndef LIBUIPF_DATA_LIST_HPP
#define LIBUIPF_DATA_LIST_HPP

#include <string>
#include <vector>
#include <map>

#include "../data.hpp"

namespace uipf {
	namespace data {


		UIPF_DATA_TYPE_BEGIN (List, "list", std::vector<Data::ptr>)

			// TODO getType() in combination

			virtual bool isList() { return true; };
			virtual std::vector<Data::ptr> getListContent() { return getContent(); };

		UIPF_DATA_TYPE_END

	};
};

#endif // LIBUIPF_DATA_LIST_HPP
