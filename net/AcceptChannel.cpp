#include "AcceptChannel.h"
#include "SocketUtil.h"

#include "common/Log.h"

AcceptChannel::AcceptChannel(int32_t fd, EventLoop* loop)
	:BaseChannel(fd, loop)
{

}

AcceptChannel::~AcceptChannel()
{

}


void AcceptChannel::OnRead()
{
	int32_t conn_fd;
	std::string user_addr;
	int32_t user_port;

	if (!SocketUtil::Accept(GetFd(), conn_fd, user_addr, user_port))
	{
		LOG_ERROR("AcceptChannel::OnRead Accept Failed err[%d]", errno);
		OnAcceptFailed();
	}

	OnAcceptSuccess(conn_fd, user_addr, user_port);
}