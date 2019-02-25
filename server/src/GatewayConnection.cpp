#include <unistd.h>
#include <netinet/in.h>

#include "GatewayConnection.h"

#include "common/Log.h"

GatewayConnection::GatewayConnection(int32_t gateway_fd, EventLoop* loop)
	:TcpChannel<ServerHeader>(gateway_fd, loop)
{

}

GatewayConnection::~GatewayConnection()
{

}

void GatewayConnection::SendServerMsg(int32_t server_type, int32_t process_id, int32_t cmd, const std::string& msg)
{
	ServerHeader header;
	header.m_server_type = server_type;
	header.m_process_id = process_id;
	header.m_cmd = cmd;
	header.m_len = msg.length();

	SendMsg((char*)&header, sizeof(ServerHeader));
	SendMsg(msg);

	LOG_INFO("GatewayConnection::SendServerMsg type[%d] pid[%d] cmd:%d size:%d send_msg:%s", server_type, process_id, cmd, header.m_len, msg.c_str());
}

void GatewayConnection::DoSocketClose()
{
	LOG_INFO("GatewayConnection::DoSocketClose");
}

void GatewayConnection::OnMessageArrived(const ServerHeader& header, const std::string& msg)
{
	LOG_INFO("GatewayConnection::OnMessageArrived type[%d] pid[%d] cmd[%d] msg[%s]", header.m_cmd,header.m_server_type, header.m_process_id, msg.c_str());
}