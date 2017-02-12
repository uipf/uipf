#ifndef LIBUIPF_DATA_HPP
#define LIBUIPF_DATA_HPP

#include <string>
#include <istream>
#include <ostream>
#include <vector>
#include <memory>

#include "exceptions.hpp"

/*
 * This file contains the Declaration for the UIPF Type system.
 *
 *
 */

// TODO
// - http://en.cppreference.com/w/cpp/language/copy_constructor
// - http://doc.qt.io/qt-5/plugins-howto.html
// // noncopyable
// Module(const Module&);
// Module& operator=(const Module&);


// reference counting smart pointer
// this is the core for allowing passing data from one module to another
#define UIPF_SMARTPOINTER std::shared_ptr

namespace uipf {

	//template <typename T>
	class Data {

	public:
		typedef UIPF_SMARTPOINTER <Data> ptr;
		typedef const UIPF_SMARTPOINTER <Data> c_ptr;

	public:
		// constructor (can't be virtual!)
		Data(void) {};

		// destructor
		virtual ~Data(void) {};

		// returns the data type of this data object
		// this is a virtual method, which has to be overwritten in the class, which derives of Data
		virtual std::string getType() const = 0;

		virtual bool isSerializable() { return false; };
		virtual bool isVisualizable() { return false; };

		// serialization, allow reading from files and writing to files
		// or other string transport
		virtual void serialize(std::ostream&) const { throw uipf::ErrorException("Data Type is not serializable."); };
		// unserialize constructor
		Data(std::istream&) { throw uipf::ErrorException("Data Type is not serializable."); };

		// TODO partitioning


	};
}

#define UIPF_DATA_TYPE_BEGIN(NAME, ID, TYPE) class NAME : public uipf::Data { \
	public: \
		typedef UIPF_SMARTPOINTER <NAME> ptr; \
		typedef const UIPF_SMARTPOINTER <NAME> c_ptr; \
		std::string getType() const override { return ID; }; \
		static std::string id() { return ID; }; \
		NAME() {}; \
		NAME(TYPE d) : data_(d) {};/* TODO constructor should not copy original data */  \
/*		NAME(const NAME& d) : data_(d.data_) {}; */ \
		~NAME(void){}; \
		TYPE& getContent() { return data_; }; \
		const TYPE& getContent() const { return data_; }; \
		void setContent(TYPE d) { data_ = d; }; \
	private: \
		TYPE data_;
#define UIPF_DATA_TYPE_END };

#endif //LIBUIPF_DATA_HPP
