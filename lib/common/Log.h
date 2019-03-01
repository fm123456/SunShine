#ifndef LOG_H__
#define LOG_H__


#include <stdarg.h>

enum LogLevel
{
	LOGFatal,    //致命错误
	LOGError,    //一般的错误
	LOGWarning,  //警告
	LOGInfo,     //信息级别
	LOGDebug,    //调试级别
};

class Log
{
public:
	virtual void LogFormat(LogLevel level, const char* fmt, ...) = 0;

	virtual void SetLevel(int32_t level) = 0;

	static Log& GetLogInstance();
};

#define SetLogLevel(x) Log::GetLogInstance().SetLevel(x)


#if 0
#define LOG_DEBUG(fmt,...) Log::GetLogInstance().LogFormat(LOGDebug, "[%s][%s:%d]" fmt,__FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__);
#define LOG_INFO(fmt,...)  Log::GetLogInstance().LogFormat(LOGInfo, "[%s][%s:%d]" fmt,__FILE__, __FUNCTION__,__LINE__,##__VA_ARGS__);
#define LOG_WARM(fmt,...)  Log::GetLogInstance().LogFormat(LOGWarning, "[%s][%s:%d]" fmt,__FILE__, __FUNCTION__,__LINE__,##__VA_ARGS__);
#define LOG_ERROR(fmt,...) Log::GetLogInstance().LogFormat(LOGInfo, "[%s][%s:%d]" fmt, __FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);
#define LOG_FATAL(fmt,...) Log::GetLogInstance().LogFormat(LOGDebug, "[%s][%s:%d]" fmt,__FILE__, __FUNCTION__,__LINE__,##__VA_ARGS__);
#else

#include <stdio.h>
template <typename ...Args>
inline void LOG_PRINT(const char* fmt, const Args& ...args)
{
	printf(fmt, args...);
	printf("\n");
}
#define LOG_DEBUG(fmt,...) LOG_PRINT(fmt, ##__VA_ARGS__);
#define LOG_INFO(fmt,...)  LOG_PRINT(fmt, ##__VA_ARGS__);
#define LOG_WARM(fmt,...)  LOG_PRINT(fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt,...) LOG_PRINT(fmt, ##__VA_ARGS__);
#define LOG_FATAL(fmt,...) LOG_PRINT(fmt, ##__VA_ARGS__);
#endif

#endif