#include "service/MasterConnection.h"

#include "common/Log.h"


MasterConnection::MasterConnection(int32_t master_fd, EventLoop* loop, const std::string& master_addr, int32_t master_port)
	:TcpChannel<MasterHeader>(master_fd, loop),m_master_addr(master_addr),m_master_port(master_port)
{

}

MasterConnection::~MasterConnection()
{

}

void MasterConnection::DoSocketClose()
{

}

void MasterConnection::OnMessageArrived(const MasterHeader& header, const std::string& msg)
{

}