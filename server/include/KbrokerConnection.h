#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include <string>
#include <stdint.h>

#include "CommonDef.h"

class EventLoop;
class SocketHandler;
class KbrokerConnection
{
public:
	KbrokerConnection(EventLoop* loop);
	~KbrokerConnection();

	bool Init();

	void SendMsg(const std::string& str);
private:
	void ReadHandler();
	void WriteHandler();

	void RegisterType();
private:
	EventLoop* m_loop;

	int32_t m_socket;
	SocketHandler* m_handler;
	ChannelPtr m_channel;
};

#endif