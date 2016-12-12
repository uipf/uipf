#include "Data.hpp"

std::string uipf::type2string(uipf::Type t) {

	switch(t) {
		case STRING:  return "STRING";
		case INTEGER: return "INTEGER";
		case FLOAT:   return "FLOAT";
		case BOOL:    return "BOOL";
		case MATRIX:  return "MATRIX";

		case STRING_LIST:  return "LIST(STRING)";
		case INTEGER_LIST: return "LIST(INTEGER)";
		case FLOAT_LIST:   return "LIST(FLOAT)";
		case BOOL_LIST:    return "LIST(BOOL)";
		case MATRIX_LIST:  return "LIST(MATRIX)";

		case CUSTOM: return "CUSTOM";
	}
	return "UNKNOWN!";
}

std::string uipf::Data::getTypeName() const {
	Type t = getType();
	if (t == CUSTOM) {
		throw std::out_of_range("CUSTOM data types must implement getTypeName() and return a unique value.");
	}
	return type2string(t);
}
