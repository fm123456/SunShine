#ifndef GATEWAY_CONNECTION_H_
#define GATEWAY_CONNECTION_H_

#include <string>
#include <stdint.h>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class GatewayConnection : public TcpChannel<ServerHeader>
{
public:
	GatewayConnection(int32_t gateway_fd, EventLoop* loop, int32_t type);
	~GatewayConnection();

	void SendServerMsg(int32_t server_type, int32_t process_id, int32_t cmd, const std::string& msg);
private:
	virtual void DoSocketClose() override;

	virtual void OnMessageArrived(const ServerHeader& header, const std::string& msg) override;

private:
	void RegisterServer(int32_t type)
};

#endif
