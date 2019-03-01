#ifndef ACCEPT_CHANNEL_H_
#define ACCEPT_CHANNEL_H_

#include <string>

#include "BaseChannel.h"

class AcceptChannel : public BaseChannel
{
public:
	AcceptChannel(int32_t fd, EventLoop* loop);
	virtual ~AcceptChannel();

private:
	virtual void OnRead() override;

	virtual void OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port) = 0;

	virtual void OnAcceptFailed() = 0;

};


#endif
