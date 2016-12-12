#ifndef UIPFRAMEWORK_DATA_HPP
#define UIPFRAMEWORK_DATA_HPP

#include <string>
#include <memory>


// reference counting smart pointer
// this is the core for allowing passing data from one module to another
#define UIPF_SMARTPOINTER std::shared_ptr

namespace uipf {

	template <typename T>
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

		// returns the value of the integer
		virtual T getContent() const = 0;

		// sets the value of the integer
		virtual void setContent(T) = 0;

		virtual std::string serialize() const = 0;
		virtual ptr unserialize(std::string) const = 0;

	};
}

#define UIPF_BEGIN_DATA_TYPE(NAME, ID, TYPE) class NAME : public uipf::Data<TYPE> { \
	public: \
		typedef UIPF_SMARTPOINTER <NAME> ptr; \
		typedef const UIPF_SMARTPOINTER <NAME> c_ptr; \
		std::string getType() const override { return ID; }; \
		NAME(TYPE d) : data_(d) {};  \
/*		NAME(const NAME& d) : data_(d.data_) {}; */ \
		~NAME(void){}; \
		TYPE getContent() const { return data_; }; \
		void setContent(TYPE d) { data_ = d; }; \
	private: \
		TYPE data_;
#define UIPF_END_DATA_TYPE };


UIPF_BEGIN_DATA_TYPE (String, "de.tu-berlin.cvrs.uipf.string", std::string)
UIPF_END_DATA_TYPE

UIPF_BEGIN_DATA_TYPE (Bool, "de.tu-berlin.cvrs.uipf.bool", bool)
UIPF_END_DATA_TYPE

UIPF_BEGIN_DATA_TYPE (Int, "de.tu-berlin.cvrs.uipf.int", int)
UIPF_END_DATA_TYPE

#endif //UIPFRAMEWORK_DATA_HPP
