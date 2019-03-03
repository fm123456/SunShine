#ifndef MASTER_CONNECTION_H_
#define MASTER_CONNECTION_H_

#include <string>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class MasterConnection : public TcpChannel<MasterHeader>
{
public:
	MasterConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port);
	~MasterConnection();

private:
	virtual void DoSocketClose() override;

	virtual void OnMessageArrived(const MasterHeader& header, const std::string& msg) override;
private:
	std::string m_addr;
	int32_t m_port;
};

#endif


