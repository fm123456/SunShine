#include <pthread.h>
#include <stdint.h>

#include "GatewayConnection.h"

#include "common/Log.h"
#include "net/EventLoop.h"
#include "net/SocketUtil.h"

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		LOG_FATAL("paramter is less");
		return 0;
	}
	std::string addr = argv[1];
	int32_t port = atoi(argv[2]);

	LOG_INFO("server addr:%s  port: %d", addr.c_str(), port);
	EventLoop ev;
	if (!ev.Init())
	{
		LOG_FATAL("Event loop init failed");
		return 0;
	}
	int32_t gateway_fd;
	if (!SocketUtil::TcpConnect(addr, port, gateway_fd))
	{
		LOG_FATAL("TcpConnect failed");
		return 0;
	}
	GatewayConnection conn(gateway_fd, &ev);
	ev.Loop();
} 