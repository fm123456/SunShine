#include "service/ProcessMessage.h"
#include "service/Service.h"
#include "service/ProcessManager.h"
#include "service/ServiceConnectManager.h"

#include "common/Log.h"

void ProcessMessage::SendMessageToGateway(int32_t fd, int32_t target_type, int32_t target_id, int32_t cmd, const std::string& msg)
{
	ServerHeader header;
	header.m_src_server_type = Service::instance().ServiceType();
	header.m_src_process_id = ProcessManager::instance().GetCurProcessId();
	header.m_target_server_type = target_type;
	header.m_target_process_id = target_id;
	header.m_cmd = cmd;
	header.m_len = msg.length();

	GatewayConnection* conn = ServiceConnectManager::instance().GetGatewayConnection(fd);
	if (!conn)
	{
		LOG_WARM("SendMessageToGateway not found the gateway connection");
		return;
	}

	conn->SendMsg((char*)&header, sizeof(ServerHeader));
	conn->SendMsg(msg);

	LOG_DEBUG("SendMessageToGateway source_type[%d] source_pid[%d] source_type[%d] source_pid[%d], cmd[%d] size[%d]",
		header.m_src_server_type, header.m_src_process_id, target_type, target_id, cmd, header.m_len);
}

void ProcessMessage::ReceiveMessageFromGateway(const ServerHeader& header, const std::string& msg)
{

}

void ProcessMessage::SendMessageToMaster(int32_t fd, int32_t cmd, const std::string& msg)
{

}

void ProcessMessage::ReceiveMessageFromMaster(const MasterHeader& header, const std::string& msg)
{

}

