#include "TestServer.h"
#include "UserAcceptor.h"

#include "common/Log.h"
#include "common/Util.h"
#include "net/SocketUtil.h"
#include "net/EventLoop.h"


TestServer::~TestServer()
{
	SAFE_DELETE(m_user_acceptor);
}

bool TestServer::Init(const std::string& listen_addr, int32_t listen_port)
{
	int32_t user_listen_fd;
	if (!SocketUtil::TcpListen(listen_addr, listen_port, user_listen_fd))
	{
		LOG_FATAL("TestServer::Init UserTcpListen Failed");
		return false;
	}
	m_loop = new EventLoop();
	if (!m_loop->Init())
	{
		LOG_FATAL("TestServer::Init Event loop init failed");
		return false;
	}
	m_user_acceptor = new UserAcceptor(user_listen_fd, m_loop);
	LOG_INFO("TestServer::Init Success");
	return true;
}

void TestServer::Loop()
{
	if (!m_loop) return;
	m_loop->Loop();
}