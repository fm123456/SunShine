#include "ListenSocket.h"
#include "UserConnection.h"
#include "ConnectManager.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/Util.h"


ListenSocket::ListenSocket(int32_t fd, EventLoop* loop)
	:AcceptChannel(fd, loop)
{

}

ListenSocket::~ListenSocket()
{

}

void ListenSocket::OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port)
{
	Util::MakeNonBlock(conn_fd);
	UserConnection* conn(new UserConnection(conn_fd, GetEventLoop(), conn_addr, conn_port));
	ConnectManager::instance().InsertUserConnection(conn_fd, conn);

	LOG_INFO("ListenSocket::OnAcceptSuccess addr:%s port:%d", conn_addr.c_str(), conn_port);
}

void ListenSocket::OnAcceptFailed()
{
	LOG_ERROR("ListenSocket::OnAcceptFailed fd:%d", GetFd());
}
