#ifndef USER_CONNECTION_H_
#define USER_CONNECTION_H_

#include <string>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class UserConnection : public TcpChannel<ClientHeader>
{
public:
	UserConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port);
	~UserConnection();

private:
	virtual void DoSocketClose() override;

	virtual void OnMessageArrived(const ClientHeader& header, char* src1, size_t size1, char* src2, size_t size2) override;
private:
	std::string m_addr;
	int32_t m_port;
};

#endif
