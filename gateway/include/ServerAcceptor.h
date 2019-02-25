#ifndef SERVER_ACCEPTOR_H_
#define SERVER_ACCEPTOR_H_


#include "net/AcceptChannel.h"

class ServerAcceptor : public AcceptChannel
{
public:
	ServerAcceptor(int32_t fd, EventLoop* loop);
	~ServerAcceptor();

private:
	virtual void OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port) override;

	virtual void OnAcceptFailed() override;
};

#endif
