#pragma once
#include <string>
#include <exception>
#include <sstream>

class ExceptionBase : public std::exception
{
public:
	ExceptionBase(std::string_view funcName, std::string_view errorDesc,
		std::string_view fileName, int line) noexcept
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
		return errorType.data();
	}
protected:
	std::string_view errorType;		// IMPORTANT: ALL STRING_VIEWs MUST BE NULL TERMINATED
private:
	std::string_view funcName;		// IMPORTANT: ALL STRING_VIEWs MUST BE NULL TERMINATED
	std::string_view errorDesc;		// IMPORTANT: ALL STRING_VIEWs MUST BE NULL TERMINATED
	std::string_view fileName;		// IMPORTANT: ALL STRING_VIEWs MUST BE NULL TERMINATED
	int line;
	mutable std::string whatBuffer;
};