#ifndef MASTER_CONNECTION_H_
#define MASTER_CONNECTION_H_


#include <string>
#include <stdint.h>

#include "net/TcpChannel.h"
#include "net/ProtocolHeader.h"


class MasterConnection : public TcpChannel<MasterHeader>
{
public:
	MasterConnection(int32_t master_fd, EventLoop* loop, const std::string& master_addr, int32_t master_port);
	~MasterConnection();

private:
	virtual void DoSocketClose() override;

	virtual void OnMessageArrived(const MasterHeader& header, const std::string& msg) override;

private:
	std::string m_master_addr;
	int32_t m_master_port;
};


#endif
