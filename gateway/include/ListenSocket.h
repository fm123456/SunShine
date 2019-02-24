#ifndef LISTEN_SOCKET_H_
#define LISTEN_SOCKET_H_

#include "net/AcceptChannel.h"

class ListenSocket : public AcceptChannel
{
public:
	ListenSocket(int32_t fd, EventLoop* loop);
	~ListenSocket();

private:
	virtual void OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port) override;

	virtual void OnAcceptFailed() override;
};

#endif
