#ifndef SERVER_CONNECTION_H_
#define SERVER_CONNECTION_H_

#include <string>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class ServerConnection : public TcpChannel<ServerHeader>
{
public:
	ServerConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port);
	~ServerConnection();

private:
	virtual void DoSocketClose() override;

	virtual void OnMessageArrived(const ServerHeader& header, const std::string& msg) override;
private:
	std::string m_addr;
	int32_t m_port;
};

#endif

