#ifndef MASTER_ACCEPTOR_H_
#define MASTER_ACCEPTOR_H_

#include "net/AcceptChannel.h"

class MasterAcceptor : public AcceptChannel
{
public:
	MasterAcceptor(int32_t fd, EventLoop* loop);
	~MasterAcceptor();

private:
	virtual void OnAcceptSuccess(int32_t conn_fd, const std::string& conn_addr, int32_t conn_port) override;

	virtual void OnAcceptFailed() override;
};

#endif
