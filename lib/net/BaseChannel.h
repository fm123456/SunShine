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

public:
	int32_t GetFd() { return m_fd; }
	EventLoop* GetEventLoop() { return m_loop; }

protected:
	void Remove();
	void EnableWrite();
	void DisableWrite();
	void EnableRead();
	void DisableRead();

private:
	virtual void OnRead() {};
	virtual void OnWrite() {};

private:
	int32_t m_fd;
	EventLoop* m_loop;
	EpollData* m_epoll_data;
};

#endif
