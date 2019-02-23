#include "ListenSocket.h"
#include "UserConnection.h"
#include "ConnectManager.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/Util.h"


ListenSocket::ListenSocket(int32_t fd, EventLoop* loop)
	:BaseChannel(fd, loop)
{

}

ListenSocket::~ListenSocket()
{

}

void ListenSocket::OnRead()
{
	int32_t conn_fd;
	std::string user_addr;
	int32_t user_port;

	if (!SocketUtil::Accept(GetFd(), conn_fd, user_addr, user_port))
	{
		LOG_ERROR("ListenSocket::OnRead Accept Failed err[%d]", errno);
		return;
	}
	Util::MakeNonBlock(conn_fd);

	LOG_INFO("ListenSocket::OnRead NewConnection addr:%s port:%d", user_addr.c_str(), user_port);
	UserConnection* conn(new UserConnection(conn_fd, GetEventLoop(), user_addr, user_port));
	ConnectManager::instance().InsertUserConnection(conn_fd, conn);
}