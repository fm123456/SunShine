#include <regex>
#include <locale>
#include <algorithm>
#include <stdarg.h>


#include "StringUtil.h"
#include "Util.h"

std::string StringUtil::ToLower(const std::string& str)
{
	std::string ret;
	ret.resize(str.length());

	std::transform(str.begin(), str.end(), ret.begin(), ::tolower);
	return ret;
}

std::string StringUtil::ToUpper(const std::string& str)
{
	std::string ret;
	ret.resize(str.length());

	std::transform(str.begin(), str.end(), ret.begin(), ::toupper);
	return ret;
}

bool StringUtil::StartWith(const std::string& str, const std::string& start_str)
{
	if (str.length() < start_str.length())
		return false;
	return str.substr(0, start_str.length()) == start_str;
}

bool StringUtil::EndWith(const std::string& str, const std::string& end_str)
{
	if (str.length() < end_str.length())
		return false;
	return str.substr(str.length() - end_str.length()) == end_str;
}

std::string StringUtil::ToString(int32_t val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(long val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(long long val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(unsigned val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(unsigned long val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(unsigned long long val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(float val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(double val)
{
	return std::to_string(val);
}
std::string StringUtil::ToString(long double val)
{
	return std::to_string(val);
}

void StringUtil::Split(const std::string& str, const std::string& split, std::vector<std::string>& result)
{
	//result = std::vector<std::string>{
	//	std::regex_token_iterator(str.begin(), str.end(), std::regex(split), -1),
	//		std::regex_token_iterator()
	//};
}

std::string StringUtil::Replace(const std::string& str, const std::string& src_string, const std::string& replace_string)
{
	return std::regex_replace(str, std::regex(src_string), replace_string);
}

void StringUtil::Format(std::string& str, const char* fmt, ...)
{
	const static int32_t default_buffer_size = 1024;
	int32_t retry_time = 1;
	
	char* buff = NULL;
	int32_t format_size = 0;
	int32_t buffer_size = 0;

	va_list ap;
	va_start(ap, fmt);

	while (true)
	{
		buffer_size = default_buffer_size * retry_time;
		buff = new char[buffer_size];
#ifdef WIN32
		format_size = _vsnprintf(buff, buffer_size, fmt, ap);
#else
		format_size = vsnprintf(buff, buffer_size, fmt, ap);
#endif // WIN32
		if (format_size < 0)
		{
			retry_time++;
			SAFE_DELETE_ARRAY(buff);
			format_size = 0;
		}
		else
		{
			break;
		}
	}
	va_end(ap);
	if (buff)
	{
		str = std::string(buff, format_size);
		SAFE_DELETE_ARRAY(buff);
	}
	return;
}