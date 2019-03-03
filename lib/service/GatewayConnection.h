#ifndef GATEWAY_CONNECTION_H_
#define GATEWAY_CONNECTION_H_

#include <string>
#include <stdint.h>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class GatewayConnection : public TcpChannel<ServerHeader>
{
public:
	GatewayConnection(int32_t gateway_fd, EventLoop* loop, const std::string& gateway_addr, int32_t gateway_port);
	~GatewayConnection();

private:
	virtual void DoSocketClose() override;
	 
	virtual void OnMessageArrived(const ServerHeader& header, const std::string& msg) override;

private:
	std::string m_gateway_addr;
	int32_t m_gateway_port;
};

#endif
