#include "ServerAcceptor.h"
#include "ServerConnection.h"
#include "ConnectManager.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/Util.h"


ServerAcceptor::ServerAcceptor(int32_t fd, EventLoop* loop)
	:AcceptChannel(fd, loop)
{

}

ServerAcceptor::~ServerAcceptor()
{

}

void ServerAcceptor::OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port)
{
	Util::MakeNonBlock(conn_fd);
	ServerConnection* conn(new ServerConnection(conn_fd, GetEventLoop(), conn_addr, conn_port));
	ConnectManager::instance().InsertServerConnection(conn_fd, conn);

	LOG_INFO("ServerAcceptor::OnAcceptSuccess addr:%s port:%d", conn_addr.c_str(), conn_port);
}

void ServerAcceptor::OnAcceptFailed()
{
	LOG_ERROR("ServerAcceptor::OnAcceptFailed fd:%d", GetFd());
}
