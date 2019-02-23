#include "GatewayServer.h"
#include "ListenSocket.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/EventLoop.h"


GatewayServer::~GatewayServer()
{
	SAFE_DELETE(m_client_socket);
}

bool GatewayServer::Init(const std::string& listen_addr, int32_t listen_port)
{
	int32_t listen_fd;
	if (!SocketUtil::TcpListen(listen_addr, listen_port, listen_fd))
	{
		LOG_FATAL("GatewayServer::Init TcpListen Failed");
		return false;
	}
	m_loop = new EventLoop();
	if (!m_loop->Init())
	{
		LOG_FATAL("GatewayServer::Init Event loop init failed");
		return false;
	}
	m_client_socket = new ListenSocket(listen_fd, m_loop);
	LOG_INFO("GatewayServer::Init Success");
	return true;
}

void GatewayServer::Loop()
{
	if (!m_loop) return;
	m_loop->Loop();
}