#include <sys/time.h>
#include "Util.h"

bool Util::MakeNonBlock(int32_t fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag == -1)
	{
		return false;
	}
	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
	{
		return false;
	}

	return true;
}