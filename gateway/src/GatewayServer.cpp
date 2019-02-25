#include "GatewayServer.h"
#include "UserAcceptor.h"
#include "ServerAcceptor.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/EventLoop.h"


GatewayServer::~GatewayServer()
{
	SAFE_DELETE(m_user_acceptor);
	SAFE_DELETE(m_server_acceptor);
}

bool GatewayServer::Init(const std::string& listen_addr, int32_t listen_port)
{
	int32_t user_listen_fd;
	if (!SocketUtil::TcpListen(listen_addr, listen_port, user_listen_fd))
	{
		LOG_FATAL("GatewayServer::Init UserTcpListen Failed");
		return false;
	}
	m_loop = new EventLoop();
	if (!m_loop->Init())
	{
		LOG_FATAL("GatewayServer::Init Event loop init failed");
		return false;
	}
	m_user_acceptor = new UserAcceptor(user_listen_fd, m_loop);
	LOG_INFO("GatewayServer::Init Success");
	return true;
}

void GatewayServer::Loop()
{
	if (!m_loop) return;
	m_loop->Loop();
}