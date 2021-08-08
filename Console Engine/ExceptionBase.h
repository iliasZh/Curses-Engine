#pragma once
#include <string>
#include <exception>
#include <sstream>

class ExceptionBase : public std::exception
{
public:
	ExceptionBase(
		std::string_view func_name_, std::string_view error_desc_,
		std::string_view file_name_, int line_) noexcept
		: funcName{ func_name_ }, errorDesc{ error_desc_ }
		, fileName{ file_name_ }, line{ line_ }
	{}
	const char* what() const noexcept override
	{
		std::stringstream ss;
		ss	<< "Error in "	<< funcName << ": " << errorDesc << "\n\n"
			<< "File: "		<< fileName	<< '\n'
			<< "Line: "		<< line		<< '\n';
		whatBuffer = ss.str();
		return whatBuffer.c_str();
	}
	const char* error_type() const noexcept { return errorType.data(); }
protected:
	std::string_view	errorType;		// ATTN: ALL STRING_VIEWs MUST BE NULL TERMINATED
private:
	std::string_view	funcName;		// ATTN: ALL STRING_VIEWs MUST BE NULL TERMINATED
	std::string_view	errorDesc;		// ATTN: ALL STRING_VIEWs MUST BE NULL TERMINATED
	std::string_view	fileName;		// ATTN: ALL STRING_VIEWs MUST BE NULL TERMINATED
	int					line;
	mutable std::string	whatBuffer;
};

