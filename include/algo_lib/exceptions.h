#pragma once

#include <exception>
#include <string>

namespace mabz {

class CustomException : public std::exception
{
private:
	std::string mMessage;

public:
	CustomException(const std::string& msg) : std::exception(), mMessage(msg) {}
	virtual const char* what() const override { return mMessage.c_str(); }
};

#define CUSTOM(name) \
	struct name : public CustomException \
	{ \
		name(const std::string& msg) : CustomException(msg) {} \
	};

CUSTOM(IndexOutOfRange)
CUSTOM(IllegalArgumentException)
CUSTOM(UnexpectedMethodCall)

#undef CUSTOM

} /* namespace mabz */