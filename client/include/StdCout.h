#ifndef STD_COUT_H__
#define STD_COUT_H__

#include <functional>

#include "common/Singleton.h"
#include "net/CommonDef.h"
#include "net/BaseChannel.h"

class SocketHandler;
class EventLoop;
class ClientConnection;
class StdCout :public BaseChannel
{
public:
	StdCout(EventLoop* loop);
	~StdCout();

private:
	virtual void OnRead() override;

private:
	void OnChat(int32_t cmd);

private:

	int32_t m_cmd;
};

#endif