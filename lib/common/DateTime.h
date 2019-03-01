#ifndef DATE_TIME_H__
#define DATE_TIME_H__

#include <stdint.h>
#include <string>

class DateTime
{
public:

	static int32_t NowSecond();

	static int64_t NowMilliSecond();

	//2018-09-10 10:00:00 
	static std::string FormatString();

	static int32_t FormatString(char* buff, size_t sz);

	//2018-09-10 10:00:00.123  �����뼶�ĸ�ʽ�� (һ����log��־�ڲ�ʹ��)
	static std::string FormatMilliString();

	static int32_t FormatMilliString(char* buff, size_t sz);
};

#endif