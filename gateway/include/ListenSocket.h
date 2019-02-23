#ifndef LISTEN_SOCKET_H_
#define LISTEN_SOCKET_H_

#include "net/BaseChannel.h"

class ListenSocket : public BaseChannel
{
public:
	ListenSocket(int32_t fd, EventLoop* loop);
	~ListenSocket();

private:
	virtual void OnRead() override;
};

#endif
