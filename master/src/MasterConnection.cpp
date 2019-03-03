#include "MasterConnection.h"
#include "common/Log.h"

MasterConnection::MasterConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port)
	:TcpChannel<MasterHeader>(fd, loop), m_addr(addr), m_port(port)
{

}

MasterConnection::~MasterConnection()
{

}

void MasterConnection::DoSocketClose()
{
	LOG_INFO("MasterConnection::DoSocketClose");
}

void MasterConnection::OnMessageArrived(const MasterHeader& header, const std::string& msg)
{
	LOG_INFO("MasterConnection::OnMessageArrived cmd[%d] size[%d] msg[%s]", header.m_cmd, header.m_len, msg.c_str());
}