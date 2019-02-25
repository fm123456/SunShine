#include "ServerConnection.h"
#include "ConnectManager.h"
#include "common/Log.h"

ServerConnection::ServerConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port)
	:TcpChannel<ServerHeader>(fd, loop), m_addr(addr), m_port(port)
{

}

ServerConnection::~ServerConnection()
{

}

void ServerConnection::DoSocketClose()
{
	LOG_INFO("ServerConnection::DoSocketClose");
	ConnectManager::instance().DeleteServerConnection(GetFd());
}

void ServerConnection::OnMessageArrived(const ServerHeader& header, const std::string& msg)
{
	LOG_INFO("ServerConnection::OnMessageArrived cmd[%d] size[%d] msg[%s]", header.m_cmd, header.m_len, msg.c_str());
}