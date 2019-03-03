#include <pthread.h>
#include <stdint.h>

#include "DataService.h"

#include "service/Service.h"
#include "common/Log.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		LOG_FATAL("paramter is less");
		return 0;
	}
	int32_t type = atoi(argv[1]);

	IProcessFactoryPtr factory(new DataProcessFactory());

	Service::instance().Start(factory, type);
	Service::instance().Loop();
	return 0;
} 