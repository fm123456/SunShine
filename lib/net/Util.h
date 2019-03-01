#ifndef COMMON_H___
#define COMMON_H___

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


class Util
{
public:
	static bool MakeNonBlock(int32_t fd);

};

#endif