#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32


//根据指针值删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif


#define CLOSE_FD(fd) \
	if (fd > 0) \
	{ \
		::close(fd); \
	}

//获取当前进程id
inline int32_t GetProcessId()
{
#ifdef WIN32
	return GetCurrentProcessId();
#else
	return getpid();
#endif // WIN32

}

#endif
