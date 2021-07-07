#pragma once
#include <string>
#include <exception>
#include <sstream>

class ExceptionBase : public std::exception
{
public:
	ExceptionBase(std::string funcName, std::string errorDesc,
		std::string fileName, int line) noexcept
		: funcName{ funcName }
		, errorDesc{ errorDesc }
		, fileName{ fileName }
		, line{ line }
	{}
	const char* what() const noexcept override
	{
		std::stringstream ss;
		ss << "Error in " << funcName << ": " << errorDesc
			<< "\n\n"
			<< "File: " << fileName << '\n'
			<< "Line: " << line << '\n';
		whatBuffer = ss.str();
		return whatBuffer.c_str();
	}
	const char* ErrorType() const noexcept
	{
		return errorType.c_str();
	}
protected:
	std::string errorType;
private:
	std::string funcName;
	std::string errorDesc;
	std::string fileName;
	int line;
	mutable std::string whatBuffer;
};