#ifndef STRING_UTIL_H__
#define STRING_UTIL_H__

#include <string>
#include <vector>
#include <stdint.h>


class StringUtil
{
public:
	static std::string ToLower(const std::string& str);

	static std::string ToUpper(const std::string& str);

	static bool StartWith(const std::string& str, const std::string& start_str);

	static bool EndWith(const std::string& str, const std::string& end_str);

	static std::string ToString(int32_t val);
	static std::string ToString(long val);
	static std::string ToString(long long val);
	static std::string ToString(unsigned val);
	static std::string ToString(unsigned long val);
	static std::string ToString(unsigned long long val);
	static std::string ToString(float val);
	static std::string ToString(double val);
	static std::string ToString(long double val);

	static void Split(const std::string& str, const std::string& split, std::vector<std::string>& result);

	static std::string Replace(const std::string& str, const std::string& src_string, const std::string& replace_string);

	static void Format(std::string& str, const char* fmt, ...);
};

#endif