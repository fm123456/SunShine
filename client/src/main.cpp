#include <pthread.h>
#include <string>


#include "net/EventLoop.h"
#include "net/SocketUtil.h"
#include "common/Log.h"

#include "ClientManager.h"

#define LOG_CONSOLE_OUTPUT 1

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		LOG_FATAL("paramter is less");
		return 0;
	}
	std::string addr = argv[1];
	int32_t port = atoi(argv[2]);

	LOG_INFO("client addr:%s  port: %d", addr.c_str(), port);
	EventLoop ev;
	if (!ev.Init())
	{
		LOG_FATAL("Event loop init failed");
		return 0;
	}
	int32_t client_fd;
	if (!SocketUtil::TcpConnect(addr, port, client_fd))
	{
		LOG_FATAL("TcpConnect failed");
		return 0;
	}
	ClientManager::instance().Init(client_fd, &ev);
	LOG_INFO("Start Event Loop");
	LOG_INFO("input cmd:");
	ev.Loop();
} 