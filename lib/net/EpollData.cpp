#include <sys/epoll.h>

#include "common/Log.h"

#include "CommonDef.h"
#include "EpollData.h"
#include "EventLoop.h"


EpollData::EpollData(int32_t fd, EventLoop* loop)
	:m_fd(fd), m_loop(loop)
{
	m_cur_status = IDLE_STATUS;
	m_events = EPOLLET;

	m_write_callback = NULL;
	m_read_callback  = NULL;
}

EpollData::~EpollData()
{

}

void EpollData::HandleEvent(int32_t epoll_event)
{
	if ((epoll_event & EPOLLIN) && m_read_callback)
	{
		m_read_callback();
	}
	else if ((epoll_event & EPOLLOUT) && m_write_callback)
	{
		m_write_callback();
	}
	else
	{
		LOG_WARM("NO EVENT HANDLE event[%d]", epoll_event);
	}
}

void EpollData::EnableWrite()
{
	if (m_events & (int32_t(EPOLLOUT)))
		return;
	m_events |= EPOLLOUT;
	Update();
}

void EpollData::DisableWrite()
{
	if (~m_events & (int32_t(EPOLLOUT)))
		return;
	m_events &= (~((int32_t)EPOLLOUT));
	Update();
}

void EpollData::EnableRead()
{
	if (m_events & (int32_t(EPOLLIN)))
		return;
	m_events |= EPOLLIN;
	Update();
}

void EpollData::DisableRead()
{
	if (~m_events & (int32_t(EPOLLIN)))
		return;
	m_events &= (~((int32_t)EPOLLIN));
	Update();
}

void EpollData::Remove()
{
	if (m_cur_status == DELETING_STATUS)
		return;
	m_write_callback = NULL;
	m_read_callback = NULL;
	m_cur_status = DELETING_STATUS;
	Update();
	LOG_DEBUG("Remove EpollData fd: %d",m_fd);
}

void EpollData::Update()
{
	m_loop->Update(this);
	if (m_cur_status == IDLE_STATUS)
		m_cur_status = RUNNING_STATUS;
}

