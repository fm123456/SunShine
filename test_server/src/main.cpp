#include "TestServer.h"

#include "common/Log.h"

int main(int argc, char** argv)
{
	//daemon(1, 0);

	if (argc < 3)
	{
		LOG_FATAL("paramter is less");
		return 0;
	}
	std::string addr = argv[1];
	int32_t port = atoi(argv[2]);
	if (!TestServer::instance().Init(addr, port))
	{
		LOG_FATAL("Server init failed");
		return -1;
	}
	TestServer::instance().Loop();

	return 0;
}