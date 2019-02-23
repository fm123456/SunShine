#include <sys/time.h>
#include <iostream>
#include <sys/time.h>
#include <strings.h>
#include <sys/timerfd.h>
#include <time.h>

#include "TimerManager.h"
#include "Util.h"
#include "Log.h"
#include "DateTime.h"


TimerInfo::TimerInfo(uint64_t _expiration, const ITimerTaskPtr& _task, uint32_t _interval)
	:m_expiration(_expiration), m_interval(_interval), m_task(_task)
{

}

TimerInfo::~TimerInfo()
{

}

void TimerInfo::Reset(int64_t now)
{
	if (!IsReapt()) return;

	m_expiration = now + m_interval;
}

void TimerInfo::Execute()
{
	//LOG_DEBUG("TimerEventData::Execute %lu", DateTime::NowMilliSecond());
	m_task->Execute();
}

TimerManager::TimerManager()
{
}

TimerManager::~TimerManager()
{
}

void TimerManager::AddMsDelayEvent(uint64_t _delay, const ITimerTaskPtr& _task, uint32_t _interval)
{
	if (_delay == 0) _delay = 1;

	int64_t now = DateTime::NowMilliSecond();
	TimerInfo* data = new TimerInfo(_delay + now, _task, _interval);
	InsertTimer(data);
}

int64_t TimerManager::GetNearestMsDelay(int64_t now_tm)
{
	if (m_timer_map.size() == 0)
		return default_timeout_ms;
	if (now_tm == 0)
		now_tm = DateTime::NowMilliSecond();

	int64_t expiration = m_timer_map.begin()->first;
	int64_t dist = expiration - now_tm;

	return dist < 0 ? 0 : dist;
}

void TimerManager::ExecuteTimerEvent(int64_t now_tm)
{
	if (now_tm == 0)
		now_tm = DateTime::NowMilliSecond();

	std::vector<TimerInfo*> reapt_timer_vec;
	for (auto it = m_timer_map.begin(); it != m_timer_map.end();)
	{
		if (it->first > now_tm)
			break;
		for (auto event_it = it->second.begin(); event_it != it->second.end(); event_it++)
		{
			(*event_it)->Execute();
			if ((*event_it)->IsReapt())
			{
				(*event_it)->Reset(now_tm);
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
}

bool TimerManager::InsertTimer(TimerInfo* data)
{
	bool need_update = false;
	if (m_timer_map.size() == 0 || m_timer_map.begin()->first > data->GetExpiration())
	{
		need_update = true;
	}
	m_timer_map[data->GetExpiration()].push_back(data);
	return need_update;
}