#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <strings.h>  // bzero
#include <string.h>
#include <stdio.h>  // snprintf
#include <unistd.h>

#include "common/Log.h"

#include "SocketUtil.h"
#include "CommonDef.h"


#define MAX_WAITING_LISTEN 32

int32_t SocketUtil::CreateSocket(bool is_noblock, bool is_tcp)
{
	int32_t fd = 0;

	int32_t flag = 0;
	if (is_noblock) flag |= SOCK_NONBLOCK;
	
	if (is_tcp)  flag |= SOCK_STREAM;
	else         flag |= SOCK_DGRAM;

	fd = ::socket(AF_INET, flag, 0);
	if (fd < 0)
	{
		LOG_DEBUG("Create Socket Error : %d", errno);
	}
	return fd;
}

int32_t SocketUtil::Bind(int32_t fd, const std::string& addr, int32_t port)
{
	struct sockaddr_in sock_addr;
	BindAddr(addr, port, sock_addr);

	return ::bind(fd, (sockaddr*)&sock_addr, sizeof(sock_addr));
}

bool SocketUtil::TcpConnect(const std::string& addr, int32_t port, int32_t& socket_fd)
{
	int32_t fd = CreateSocket(false);
	if (fd < 0)
	{
		LOG_ERROR("TcpConnect Create Socket Failed err:%d addr:%s port:%d", errno, addr.c_str(),port);
		return false;
	}
	struct sockaddr_in server_addr;
	BindAddr(addr, port, server_addr);

	int32_t timeouts[] = { 1,2,4,8,8,8 };
	int32_t try_time = 0;
	while (try_time <= 5)
	{
		if (::connect(fd, (sockaddr*)(&server_addr), sizeof(server_addr)) < 0)
		{
			LOG_ERROR("try connect failed try_times:%d errno:%d addr:%s port:%d", try_time + 1, errno, addr.c_str(), port);
			sleep(timeouts[try_time]);
			try_time++;
			continue;
		}
		break;
	}
	if (try_time > 5)
	{
		LOG_ERROR("Connect Failed Now Return");
		return false;
	}
	LOG_INFO("Connect Server Success addr:%s port:%d", addr.c_str(), port);

	socket_fd = fd;
	return true;
}

void SocketUtil::BindAddr(const std::string& addr, int32_t port, struct sockaddr_in& sock_addr)
{
	memset((void*)&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	inet_pton(AF_INET, addr.c_str(), &sock_addr.sin_addr);
	sock_addr.sin_port = htons(port);
}

bool SocketUtil::TcpListen(const std::string& addr, int32_t port, int32_t& socket_fd)
{
	int32_t fd = CreateSocket();
	if (fd < 0)
	{
		LOG_ERROR("TcpListen Create Socket Failed addr:%s port:%d errno:%d", addr.c_str(), port, errno);
		return false;
	}
	SetReuseAddr(fd, true);
	SetReusePort(fd, true);
	if (Bind(fd, addr, port) < 0)
	{
		LOG_ERROR("TcpListen  Bind Socket Failed addr:%s port:%d errno:%d", addr.c_str(), port, errno);
		return false;
	}
	if (::listen(fd, MAX_WAITING_LISTEN) < 0)
	{
		LOG_ERROR("TcpListen Listen Socket Failed addr:%s port:%d errno:%d", addr.c_str(), port, errno);
		return false;
	}

	socket_fd = fd;
	LOG_INFO("TcpListen Listen Socket Success addr:%s port:%d", addr.c_str(), port);
	return true;
}

bool SocketUtil::Accept(int32_t fd, int32_t& connect_fd, std::string& connect_addr, int32_t& connect_port)
{
	struct sockaddr_in client_info;
	socklen_t len = sizeof(client_info);

	int32_t conn_fd = ::accept(fd, (sockaddr*)&client_info, &len);
	if (conn_fd < 0)
	{
		LOG_ERROR("SocketUtil::Accept Accept Failed err[%d]", errno);
		return false; 
	}
	connect_addr = inet_ntoa(client_info.sin_addr);
	connect_port = ntohs(client_info.sin_port);
	connect_fd = conn_fd;

	return true;
}

void SocketUtil::SetTcpNoDelay(int32_t fd, bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void SocketUtil::SetReuseAddr(int32_t fd, bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void SocketUtil::SetReusePort(int32_t fd, bool on)
{
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {
    LOG_DEBUG("SO_REUSEPORT failed.");
  }
#else
  if (on)
  {
    LOG_DEBUG("SO_REUSEPORT is not supported.");
  }
#endif
}

void SocketUtil::SetKeepAlive(int32_t fd, bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void SocketUtil::SetTcpLinger(int32_t fd, short u_onoff, short u_linger)
{
	linger lg;
	lg.l_onoff = u_onoff;
	lg.l_linger = u_linger;
	::setsockopt(fd,SOL_SOCKET, SO_LINGER, (void*)&lg, static_cast<socklen_t>(sizeof lg));
}

