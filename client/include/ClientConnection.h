#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include <string>
#include <stdint.h>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class ClientConnection : public TcpChannel<ClientHeader>
{
public:
	ClientConnection(int32_t client_fd, EventLoop* loop);
	~ClientConnection();

	void SendServerMsg(int32_t cmd, const std::string& msg);
private:
	virtual void DoSocketClose() override;

	virtual void OnMessageArrived(const ClientHeader& header, const std::string& msg) override;
};

#endif