#include "UserAcceptor.h"
#include "UserConnection.h"
#include "ConnectManager.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/Util.h"


UserAcceptor::UserAcceptor(int32_t fd, EventLoop* loop)
	:AcceptChannel(fd, loop)
{

}

UserAcceptor::~UserAcceptor()
{

}

void UserAcceptor::OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port)
{
	Util::MakeNonBlock(conn_fd);
	UserConnection* conn(new UserConnection(conn_fd, GetEventLoop(), conn_addr, conn_port));
	ConnectManager::instance().InsertUserConnection(conn_fd, conn);

	LOG_INFO("UserAcceptor::OnAcceptSuccess addr:%s port:%d", conn_addr.c_str(), conn_port);
}

void UserAcceptor::OnAcceptFailed()
{
	LOG_ERROR("UserAcceptor::OnAcceptFailed fd:%d", GetFd());
}
