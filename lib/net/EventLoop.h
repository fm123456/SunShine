#ifndef EVENT_LOOP_H__
#define EVENT_LOOP_H__

#include <memory>
#include <stdint.h>

class EpollData;
class EventLoop
{
public:
	EventLoop();
	~EventLoop();

	bool Init();
	void Loop();

	void Update(EpollData* epoll_data);

private:
	void UpdateEvent(int32_t event_type, EpollData* epoll_data);

private:
	int32_t m_epoll_fd;
};

#endif