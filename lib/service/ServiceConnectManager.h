#ifndef SERVICE_CONNECT_MANAGER_H_
#define SERVICE_CONNECT_MANAGER_H_

#include <map>
#include <stdint.h>

#include "service/GatewayConnection.h"
#include "service/MasterConnection.h"

#include "common/Singleton.h"
#include "net/ProtocolHeader.h"

class EventLoop;

class ServiceConnectManager : public Singleton<ServiceConnectManager>
{
public:
	ServiceConnectManager();
	~ServiceConnectManager();

	bool Init(EventLoop* loop);

	void InsertGatewayConnection(int32_t id, GatewayConnection* conn);

	void RemoveGatewayConnection(int32_t id);

	void InsertMasterConnection(int32_t id, MasterConnection* conn);

	void RemoveMasterConnection(int32_t id);

	MasterConnection* GetMasterConnection(int32_t fd);

	GatewayConnection* GetGatewayConnection(int32_t fd);
private:

	bool ConnectGateway(EventLoop* loop, std::map<std::string, int32_t>& gateway_list);

	bool ConnectMaster(EventLoop* loop, std::map<std::string, int32_t>& master_list);

	//想网关注册自己的服务类型
	bool RegisterTypeToGateway();
private:
	std::map<int32_t, MasterConnection*> m_master_map;
	std::map<int32_t, GatewayConnection*> m_gateway_map;

};


#endif
