#include <chrono>
#include <time.h>
#include "DateTime.h"

static char format_array[100][3] = { "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
									"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
									"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
									"30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
									"40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
									"50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
									"60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
									"70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
									"80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
									"90", "91", "92", "93", "94", "95", "96", "97", "98", "99" };

int32_t DateTime::NowSecond()
{
	return int32_t(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

int64_t DateTime::NowMilliSecond()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//2018-09-10 10:00:00
#define FORMAT_SECOND_LEN 19
std::string DateTime::FormatString()
{
	std::string format_string;
	format_string.resize(FORMAT_SECOND_LEN);
	time_t now = NowSecond();
	struct tm* now_tm = localtime(&now);
	snprintf(&format_string[0], format_string.length(), "%d-%s-%s %s:%s:%s",
		now_tm->tm_year + 1900,
		format_array[now_tm->tm_mon + 1],
		format_array[now_tm->tm_mday],
		format_array[now_tm->tm_hour],
		format_array[now_tm->tm_min],
		format_array[now_tm->tm_sec]);

	return format_string;
}

int32_t DateTime::FormatString(char* buff, size_t sz)
{
	if (sz < FORMAT_SECOND_LEN)
		return 0; 

	time_t now = NowSecond();
	struct tm* now_tm = localtime(&now);
	int32_t len = snprintf(buff, FORMAT_SECOND_LEN, "%d-%s-%s %s:%s:%s",
		now_tm->tm_year + 1900,
		format_array[now_tm->tm_mon + 1],
		format_array[now_tm->tm_mday],
		format_array[now_tm->tm_hour],
		format_array[now_tm->tm_min],
		format_array[now_tm->tm_sec]);

	return len;
}

//2018-09-10 10:00:00.321
#define FORMAT_MILLI_SECOND_LEN 23
std::string DateTime::FormatMilliString()
{
	std::string format_string;
	format_string.resize(FORMAT_MILLI_SECOND_LEN);

	int64_t milli_second = NowMilliSecond();
	time_t now_sec = milli_second / 1000;
	int64_t remain_milli_sec = milli_second - now_sec * 1000;

	struct tm* now_tm = localtime(&now_sec);
	if (remain_milli_sec >= 100)
	{
		snprintf(&format_string[0], format_string.length(), "%d-%s-%s %s:%s:%s.%lu",
			now_tm->tm_year + 1900,
			format_array[now_tm->tm_mon + 1],
			format_array[now_tm->tm_mday],
			format_array[now_tm->tm_hour],
			format_array[now_tm->tm_min],
			format_array[now_tm->tm_sec],
			remain_milli_sec);
	}
	else
	{
		snprintf(&format_string[0], format_string.length(), "%d-%s-%s %s:%s:%s.0%s",
			now_tm->tm_year + 1900,
			format_array[now_tm->tm_mon + 1],
			format_array[now_tm->tm_mday],
			format_array[now_tm->tm_hour],
			format_array[now_tm->tm_min],
			format_array[now_tm->tm_sec],
			format_array[remain_milli_sec]);
	}
	return format_string;
}

int32_t DateTime::FormatMilliString(char* buff, size_t sz)
{
	if (sz < FORMAT_MILLI_SECOND_LEN)
		return 0;

	int64_t milli_second = NowMilliSecond();
	time_t now_sec = milli_second / 1000;
	int64_t remain_milli_sec = milli_second - now_sec * 1000;
	struct tm* now_tm = localtime(&now_sec);

	int32_t len = 0;
	if (remain_milli_sec >= 100)
	{
		len = snprintf(buff, sz, "%d-%s-%s %s:%s:%s.%lu",
			now_tm->tm_year + 1900,
			format_array[now_tm->tm_mon + 1],
			format_array[now_tm->tm_mday],
			format_array[now_tm->tm_hour],
			format_array[now_tm->tm_min],
			format_array[now_tm->tm_sec],
			remain_milli_sec);
	}
	else
	{
		len = snprintf(buff, sz, "%d-%s-%s %s:%s:%s.0%s",
			now_tm->tm_year + 1900,
			format_array[now_tm->tm_mon + 1],
			format_array[now_tm->tm_mday],
			format_array[now_tm->tm_hour],
			format_array[now_tm->tm_min],
			format_array[now_tm->tm_sec],
			format_array[remain_milli_sec]);
	}

	return len;
}