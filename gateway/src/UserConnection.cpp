#include "UserConnection.h"
#include "common/Log.h"

UserConnection::UserConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port)
	:BaseChannel(fd, loop),m_addr(addr),m_port(port)
{

}

UserConnection::~UserConnection()
{

}

void UserConnection::OnRead()
{
	LOG_INFO("UserConnection::OnRead");
}

void UserConnection::OnWrite()
{

}