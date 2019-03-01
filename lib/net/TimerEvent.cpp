#include <sys/time.h>
#include <iostream>
#include <sys/time.h>
#include <strings.h>
#include <sys/timerfd.h>
#include <time.h>

#include "common/Log.h"
#include "common/DateTime.h"
#include "common/Util.h"

#include "EpollData.h"
#include "TimerEvent.h"
#include "Util.h"




TimerEventData::TimerEventData(uint64_t _expiration, const Task& _task, uint32_t _interval)
	:m_expiration(_expiration),m_task(_task),m_interval(_interval)
{
	
}

TimerEventData::~TimerEventData()
{

}

void TimerEventData::Reset(int64_t now)
{
	if(!IsReapt()) return;

	m_expiration = now + m_interval;
}

void TimerEventData::Execute()
{
	//LOG_DEBUG("TimerEventData::Execute %lu", DateTime::NowMilliSecond());
	m_task();
}

TimerEventManager::TimerEventManager(EventLoop* loop)
	:m_loop(loop)
{
	m_timerfd = 0;
	m_epoll_data = NULL;
}

TimerEventManager::~TimerEventManager()
{
	CLOSE_FD(m_timerfd);
	SAFE_DELETE(m_epoll_data);
}

bool TimerEventManager::Init()
{
	m_timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (m_timerfd < 0)
	{
		return false;
	}

	m_epoll_data = new EpollData(m_timerfd, m_loop);
	m_epoll_data->SetReadCallback(std::bind(&TimerEventManager::HandRead, this));
	m_epoll_data->EnableRead();
	return true;
}

void TimerEventManager::AddMsDelayEvent(uint64_t _delay, const Task& _task, uint32_t _interval)
{
	if (_delay == 0) _delay = 1;

	int64_t now = DateTime::NowMilliSecond();
	TimerEventData* data  = new TimerEventData(_delay+now, _task, _interval);
	
	if (InsertTimer(data))
	{
		UpdateTimer(now);
	}
}

void TimerEventManager::HandRead()
{
	int64_t now = DateTime::NowMilliSecond();
	LOG_DEBUG("TimerEventManager::HandRead new: %lu",now);
	std::vector<TimerEventData*> reapt_timer_vec;
	ReadTimerFd();
	for (auto it = m_timer_map.begin(); it != m_timer_map.end();)
	{
		if (it->first > now)
			break;
		for (auto event_it = it->second.begin(); event_it != it->second.end(); event_it++)
		{
			(*event_it)->Execute();
			if ((*event_it)->IsReapt())
			{
				(*event_it)->Reset(now);
				reapt_timer_vec.push_back(*event_it);
			}
			else
				SAFE_DELETE(*event_it);
		}
		it = m_timer_map.erase(it);
	}
	for (auto it = reapt_timer_vec.begin(); it != reapt_timer_vec.end(); it++)
	{
		InsertTimer(*it);
	}
	UpdateTimer(now);
}

void TimerEventManager::ReadTimerFd()
{
	uint64_t data;
	int32_t read_size = ::read(m_timerfd, (void*)&data, sizeof(uint64_t));
	LOG_DEBUG("TimerEventManager ReadTimerFd  readsize: %d  data:lu", read_size, data);
}

bool TimerEventManager::InsertTimer(TimerEventData* data)
{
	bool need_update = false;
	if (m_timer_map.size() == 0 || m_timer_map.begin()->first > data->GetExpiration())
	{
		need_update = true;
	}
	m_timer_map[data->GetExpiration()].push_back(data);
	return need_update;
}

void TimerEventManager::UpdateTimer(int64_t now)
{
	if (m_timer_map.size() == 0)
		return;
	int64_t expiration = m_timer_map.begin()->first;
	int64_t dist = expiration - now;
	if (dist <= 0)
		return;

	LOG_DEBUG("TimerEventManager UpdateTimer s:%d ns:%d",dist/1000,dist%1000);
	struct itimerspec newValue;
	bzero(&newValue, sizeof newValue);
	newValue.it_value.tv_sec  =  dist/1000;
	newValue.it_value.tv_nsec = (dist%1000) * 1000 * 1000 ; 
	int ret = ::timerfd_settime(m_timerfd, 0, &newValue, NULL);
	if (ret)
	{
		LOG_DEBUG("Settimer Error");
	}
}