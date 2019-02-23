#ifndef USER_CONNECTION_H_
#define USER_CONNECTION_H_

#include <string>

#include "net/BaseChannel.h"

class UserConnection : public BaseChannel
{
public:
	UserConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port);
	~UserConnection();

private:
	virtual void OnRead() override;
	virtual void OnWrite() override;

private:
	std::string m_addr;
	int32_t m_port;
};

#endif
