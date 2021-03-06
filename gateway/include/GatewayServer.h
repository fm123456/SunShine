#ifndef GATEWAY_SERVER_H_
#define GATEWAY_SERVER_H_

#include <string>
#include <stdint.h>

#include "common/Singleton.h"

class EventLoop;
class UserAcceptor;
class ServerAcceptor;
class GatewayServer : public Singleton<GatewayServer>
{
public:
	~GatewayServer();

	bool Init(const std::string& listen_addr, int32_t listen_port);

	void Loop();

private:
	EventLoop* m_loop = NULL;
	UserAcceptor* m_user_acceptor = NULL;
	ServerAcceptor* m_server_acceptor = NULL;
};

#endif
