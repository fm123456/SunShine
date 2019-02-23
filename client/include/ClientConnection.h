#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include <string>
#include <stdint.h>

#include "net/BaseChannel.h"


class ClientConnection : public BaseChannel
{
public:
	ClientConnection(int32_t client_fd, EventLoop* loop);
	~ClientConnection();

private:
	virtual void OnRead() override;
	virtual void OnWrite() override;
};

#endif