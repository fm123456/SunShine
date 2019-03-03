#include "MasterAcceptor.h"
#include "MasterConnection.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/Util.h"


MasterAcceptor::MasterAcceptor(int32_t fd, EventLoop* loop)
	:AcceptChannel(fd, loop)
{

}

MasterAcceptor::~MasterAcceptor()
{

}

void MasterAcceptor::OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port)
{
	Util::MakeNonBlock(conn_fd);
	//MasterConnection* conn(new MasterConnection(conn_fd, GetEventLoop(), conn_addr, conn_port));

	LOG_INFO("MasterAcceptor::OnAcceptSuccess addr:%s port:%d", conn_addr.c_str(), conn_port);
}

void MasterAcceptor::OnAcceptFailed()
{
	LOG_ERROR("MasterAcceptor::OnAcceptFailed fd:%d", GetFd());
}
