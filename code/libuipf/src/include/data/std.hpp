#ifndef LIBUIPF_DATA_STD_HPP
#define LIBUIPF_DATA_STD_HPP

#include <string>
#include <vector>
#include <map>

#include "../data.hpp"

namespace uipf {
	namespace data {

		// define default types that ship with UIPF

		UIPF_DATA_TYPE_BEGIN (String, "uipf.String", std::string)

			bool isSerializable() const { return true; };

			void serialize(std::ostream& stream) const {
				stream << getContent();
			}
			String(std::istream& stream) {
				std::string c;
				stream >> c;
				setContent(c);
			}
		UIPF_DATA_TYPE_END

		UIPF_DATA_TYPE_BEGIN (Bool, "uipf.Bool", bool)
		UIPF_DATA_TYPE_END

		UIPF_DATA_TYPE_BEGIN (Int, "uipf.Int", int)
		UIPF_DATA_TYPE_END

		UIPF_DATA_TYPE_BEGIN (Float, "uipf.Float", float)
		UIPF_DATA_TYPE_END

		UIPF_DATA_TYPE_BEGIN (Double, "uipf.Double", double)
		UIPF_DATA_TYPE_END

		UIPF_DATA_TYPE_BEGIN (StringList, "uipf.StringList", std::vector<std::string>)
		UIPF_DATA_TYPE_END

		UIPF_DATA_TYPE_BEGIN (IntList, "uipf.StringList", std::vector<int>)
		UIPF_DATA_TYPE_END

	};
};

#endif // LIBUIPF_DATA_STD_HPP
