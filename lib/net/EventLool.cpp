#include <sys/epoll.h>
#include <strings.h>

#include "common/Log.h"
#include "common/Util.h"

#include "CommonDef.h"
#include "EventLoop.h"
#include "EpollData.h"


#define MAX_EPOLL_SIZE 32
#define MAX_EPOLL_FD MAX_EPOLL_SIZE-1

EventLoop::EventLoop()
{
	m_epoll_fd = 0;
}

EventLoop::~EventLoop()
{
	CLOSE_FD(m_epoll_fd);
}

bool EventLoop::Init()
{
	m_epoll_fd = epoll_create(MAX_EPOLL_SIZE);
	if (m_epoll_fd < 0)
	{
		LOG_DEBUG("Create Epoll fd failed error[%d]", errno);
		return false;
	}
	return true;
}

void EventLoop::Loop()
{
	struct epoll_event evs[MAX_EPOLL_FD];

	while(true)
	{
		int32_t num = ::epoll_wait(m_epoll_fd, evs, MAX_EPOLL_FD, -1);
		for (int32_t i = 0; i < num; i++)
		{
			EpollData* epoll_data = static_cast<EpollData*>(evs[i].data.ptr);
			epoll_data->HandleEvent(evs[i].events);
		}
	}
}

void EventLoop::Update(EpollData* epoll_data)
{
	int32_t status = epoll_data->GetStatus();
	 if (status == RUNNING_STATUS)
	{
		UpdateEvent(EPOLL_CTL_MOD, epoll_data);
	}
	else if (status == IDLE_STATUS)
	{
		UpdateEvent(EPOLL_CTL_ADD, epoll_data);
	}
	else
	{
		UpdateEvent(EPOLL_CTL_DEL, epoll_data);
	}
}

void EventLoop::UpdateEvent(int32_t event_type, EpollData* epoll_data)
{
	struct epoll_event ev;
	bzero(&ev, sizeof ev);
	ev.events = epoll_data->GetEvents();
	ev.data.ptr = epoll_data;

	if (::epoll_ctl(m_epoll_fd, event_type, epoll_data->GetFd(), &ev) < 0)
	{
		LOG_DEBUG("epoll_ctl failed type[%d] err[%d]",  event_type, errno);
	}
}