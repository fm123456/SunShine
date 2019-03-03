#include <unistd.h>
#include <netinet/in.h>

#include "GatewayConnection.h"

#include "common/Log.h"
#include "common/ServerType.h"
#include "proto/ProtoDefine.h"
#include "ServiceConnectManager.h"


GatewayConnection::GatewayConnection(int32_t gateway_fd, EventLoop* loop, const std::string& gateway_addr, int32_t gateway_port)
	:TcpChannel<ServerHeader>(gateway_fd, loop), m_gateway_addr(gateway_addr),m_gateway_port(gateway_port)
{
}

GatewayConnection::~GatewayConnection()
{

}

void GatewayConnection::DoSocketClose()
{
	LOG_INFO("GatewayConnection::DoSocketClose gateway_addr:%s gateway_port:%d", m_gateway_addr.c_str(), m_gateway_port);
	ServiceConnectManager::instance().RemoveGatewayConnection(GetFd());
}

void GatewayConnection::OnMessageArrived(const ServerHeader& header, const std::string& msg)
{
	LOG_INFO("GatewayConnection::OnMessageArrived type[%d] pid[%d] cmd[%d] msg[%s]", header.m_cmd,header.m_src_server_type, header.m_src_process_id, msg.c_str());
}