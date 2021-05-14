#pragma once
#include <string>
#include <exception>
#include <sstream>

class ExceptionBase : public std::exception
{
public:
	ExceptionBase(std::string funcname, std::string errorDesc,
		std::string filename, int line) noexcept
		: funcname{ funcname }
		, errorDesc{ errorDesc }
		, filename{ filename }
		, line{ line }
	{}
	const char* what() const noexcept override
	{
		std::stringstream ss;
		ss << "Error in " << funcname << ": " << errorDesc
			<< "\n\n"
			<< "File: " << filename << '\n'
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
	std::string funcname;
	std::string errorDesc;
	std::string filename;
	int line;
	mutable std::string whatBuffer;
};