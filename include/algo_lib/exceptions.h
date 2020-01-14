#pragma once

#include <exception>
#include <string>

namespace mabz {

#define CUSTOM(name) \
class name : public std::exception \
{ \
private: \
	std::string mMessage; \
\
public: \
	name(const std::string& msg) : std::exception(), mMessage(msg) {} \
	virtual const char* what() const override { return mMessage.c_str(); } \
};

CUSTOM(IndexOutOfRange)
CUSTOM(IllegalArgumentException)
CUSTOM(UnexpectedMethodCall)
CUSTOM(Unreachable)

#undef CUSTOM

} /* namespace mabz */