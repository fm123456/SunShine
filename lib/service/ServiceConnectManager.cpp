#include "service/ServiceConnectManager.h"
#include "service/Service.h"

#include "ProcessManager.h"

#include "common/Util.h"
#include "net/SocketUtil.h"
#include "proto/ProtoDefine.h"


#define DELETE_CONNECTION_ARRAY(connection_map) \
			for (auto it : connection_map)\
			{\
				SAFE_DELETE(it.second);\
			}\
			connection_map.clear();

ServiceConnectManager::ServiceConnectManager()
{
	
}

ServiceConnectManager::~ServiceConnectManager()
{
	DELETE_CONNECTION_ARRAY(m_gateway_map);
	DELETE_CONNECTION_ARRAY(m_master_map);
}

bool ServiceConnectManager::Init(EventLoop* loop)
{
	std::map<std::string, int32_t> gateway_list;
	if (!ConnectGateway(loop, gateway_list))
		return false;
	std::map<std::string, int32_t> master_list;
	if (!ConnectMaster(loop, master_list))
		return false;
	if (!RegisterTypeToGateway())
		return false;

	return true;
}

void ServiceConnectManager::InsertGatewayConnection(int32_t id, GatewayConnection* conn)
{
	m_gateway_map[id] = conn;
}

void ServiceConnectManager::RemoveGatewayConnection(int32_t id)
{
	auto it = m_gateway_map.find(id);
	if (it != m_gateway_map.end())
	{
		m_gateway_map.erase(it);
	}
}

void ServiceConnectManager::InsertMasterConnection(int32_t id, MasterConnection* conn)
{
	m_master_map[id] = conn;
}

void ServiceConnectManager::RemoveMasterConnection(int32_t id)
{
	auto it = m_master_map.find(id);
	if (it != m_master_map.end())
	{
		m_master_map.erase(it);
	}
}

bool ServiceConnectManager::ConnectGateway(EventLoop* loop, std::map<std::string, int32_t>& gateway_list)
{
	if (gateway_list.empty())
	{
		LOG_FATAL("ServiceConnectManager no usable gateway, maybe the config is wrong");
		return false;
	}

	LOG_INFO("ServiceConnectManager will connect %d gateway", gateway_list.size());

	int32_t index = 0;
	for (auto it : gateway_list)
	{
		int32_t gateway_fd;
		if (!SocketUtil::TcpConnect(it.first, it.second, gateway_fd))
		{
			LOG_FATAL("ServiceConnectManager connect gateway failed gateway_addr:%s gateway_port:%d", it.first.c_str(), it.second);
			return false;
		}
		GatewayConnection* conn = new GatewayConnection(gateway_fd, loop, it.first, it.second);
		InsertGatewayConnection(gateway_fd, conn);
		LOG_INFO("ConnectGateway connect gateway success addr:%s port:%d index:%d", it.first.c_str(), it.second, ++index);
	}
	return true;
}

bool ServiceConnectManager::ConnectMaster(EventLoop* loop, std::map<std::string, int32_t>& master_list)
{
	if (master_list.empty())
	{
		LOG_FATAL("ServiceConnectManager no usable master, maybe the config is wrong");
		return false;
	}

	LOG_INFO("ServiceConnectManager will connect %d master", master_list.size());

	int32_t index = 0;
	for (auto it : master_list)
	{
		int32_t master_fd;
		if (!SocketUtil::TcpConnect(it.first, it.second, master_fd))
		{
			LOG_FATAL("ServiceConnectManager connect master failed master_addr:%s master_port:%d", it.first.c_str(), it.second);
			return false;
		}
		MasterConnection* conn = new MasterConnection(master_fd, loop, it.first, it.second);
		InsertMasterConnection(master_fd, conn);
		LOG_INFO("ServiceConnectManager connect master success addr:%s port:%d index:%d", it.first.c_str(), it.second, ++index);
	}
	return true;
}

bool ServiceConnectManager::RegisterTypeToGateway()
{
	SSProto::RegisterServer register_type;
	register_type.set_type(Service::instance().ServiceType());
	std::string msg;
	if (!register_type.SerializeToString(&msg))
	{
		LOG_FATAL("ServiceConnectManager::RegisterTypeToGateway SerializeToString failed");
		return false;
	}

	ServerHeader header;
	header.m_cmd = REGISTER_TYPE_CMD;
	header.m_len = msg.length();
	for (auto it : m_gateway_map)
	{
		it.second->SendMsg((char*)&header, sizeof(ServerHeader));
		it.second->SendMsg(msg);
	}
	LOG_INFO("ServiceConnectManager::RegisterTypeToGateway Register Type has send");
	return true;
}

MasterConnection* ServiceConnectManager::GetMasterConnection(int32_t fd)
{
	auto it = m_master_map.find(fd);
	return it != m_master_map.end() ? it->second : nullptr;
}

GatewayConnection* ServiceConnectManager::GetGatewayConnection(int32_t fd)
{
	auto it = m_gateway_map.find(fd);
	return it != m_gateway_map.end() ? it->second : nullptr;
}
