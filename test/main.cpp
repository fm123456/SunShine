#include <iostream>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <map>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common/StringUtil.h"
#include "common/DateTime.h"
#include "net/CommonDef.h"

void Func()
{

}


class Base
{
public:

};

#define DEFINE_NEW_CLASS(classname) class classname : public Base\
	{\
		public:\
		int classname_num = 1; \
	};

DEFINE_NEW_CLASS(AAAA);
DEFINE_NEW_CLASS(BBBB);
DEFINE_NEW_CLASS(CCCC);

template<class T, class TBase>
std::type_index GetTypeIndex(TBase* ptr)
{
	TBase* data = static_cast<TBase*>(ptr);
	return std::type_index(typeid(*data));
}

struct MyStruct
{
	int32_t m_x;
	int32_t m_y;
	int32_t m_z;
};

#define MEMORY_ALLOCATOR

int main()
{
	MyStruct a;
	
	pid_t pid = getpid();
	char strProcessPath[1024] = { 0 };
	if (::readlink("/proc/self/exe", strProcessPath, 1024) <= 0)
	{
		return -1;
	}

	char *strProcessName = strrchr(strProcessPath, '/');

	if (!strProcessName)
	{
		printf("id��%d\n", pid);
		printf("name��\n");
		printf("path��%s\n", strProcessPath);
	}
	else
	{
		std::string name = std::string(++strProcessName);
		printf("id��%d\n", pid);
		printf("name��%s\n", name.c_str());
		printf("path��%s\n", strProcessPath);
	}

	return 0;


	const int32_t array_size = 100000;
	
#ifndef MEMORY_ALLOCATOR
	MyStruct* a[array_size];
	for (int i = 0; i< array_size;i++)
		a[i] = new MyStruct;

	int64_t begin = DateTime::NowMilliSecond();
	for (int32_t n = 0; n < 1000; n++)
	{
		for (int32_t i = 0; i < array_size; i++)
		{
			a[i]->m_x = sqrt(a[i]->m_x*a[i]->m_x + a[i]->m_y*a[i]->m_y + a[i]->m_z*a[i]->m_z);
		}
	}
	std::cout << DateTime::NowMilliSecond() - begin << std::endl;
#else
	MyStruct a[array_size];
	int64_t begin = DateTime::NowMilliSecond();
	for (int32_t n = 0; n < 1000; n++)
	{
		for (int32_t i = 0; i < array_size; i++)
		{
			a[i].m_x = sqrt(a[i].m_x*a[i].m_x + a[i].m_y*a[i].m_y + a[i].m_z*a[i].m_z);
		}
	}
	std::cout << DateTime::NowMilliSecond() - begin << std::endl;
#endif
	return 0;
}