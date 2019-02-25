#include "UserConnection.h"
#include "ConnectManager.h"
#include "common/Log.h"

UserConnection::UserConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port)
	:TcpChannel<ClientHeader>(fd, loop),m_addr(addr),m_port(port)
{

}

UserConnection::~UserConnection()
{

}

void UserConnection::DoSocketClose()
{
	LOG_INFO("UserConnection::DoSocketClose");
	ConnectManager::instance().DeleteUserConnection(GetFd());
}

void UserConnection::OnMessageArrived(const ClientHeader& header, const std::string& msg)
{
	LOG_INFO("UserConnection::OnMessageArrived cmd[%d] size[%d] msg[%s]", header.m_cmd, header.m_len, msg.c_str());
}