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

class IndexOutOfRange : public CustomException
{
public:
	IndexOutOfRange(const std::string& msg) : CustomException(msg) {}
};

class IllegalArgumentException : public CustomException
{
public:
    IllegalArgumentException(const std::string& msg) : CustomException(msg) {}
};

} /* namespace mabz */