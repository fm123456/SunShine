#ifndef BASE_CHANNEL_H_
#define BASE_CHANNEL_H_

#include <stdint.h>

class EventLoop;
class SocketHandler;
class EpollData;
class BaseChannel
{
public:
	BaseChannel(int32_t fd, EventLoop* loop);
	virtual ~BaseChannel();

	void EnableWrite();
	void DisableWrite();
	void EnableRead();
	void DisableRead();

	int32_t GetFd() { return m_fd; }
	EventLoop* GetEventLoop() { return m_loop; }
private:
	virtual void OnRead() {};
	virtual void OnWrite() {};

private:
	int32_t m_fd;
	EventLoop* m_loop;

	SocketHandler* m_handler;
	EpollData* m_epoll_data;
};

#endif
