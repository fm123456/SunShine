#ifndef FK_TIME_EVENT_H_
#define FK_TIME_EVENT_H_

#include <stdint.h>
#include <vector>
#include <map>

#include "CommonDef.h"

class EventLoop;
class TimerEventData
{
public:
	TimerEventData(uint64_t _expiration, const Task& _task, uint32_t _interval = 0);
	~TimerEventData();

	int64_t GetExpiration(){return m_expiration;}
	bool IsReapt(){return m_interval != 0;}

	void Reset(int64_t now);
	void Execute();

private:
	uint64_t m_expiration;
	Task m_task;
	uint32_t m_interval;
};
typedef std::vector<TimerEventData*> TimerList;
typedef std::map<int64_t, TimerList> TimerDataMap;

class TimerEventManager
{
public:
	TimerEventManager(EventLoop* loop);
	~TimerEventManager();

	bool Init();

	void AddMsDelayEvent(uint64_t _delay, const Task& _task, uint32_t _interval = 0);

private:
	void HandRead();
	void ReadTimerFd();

	bool InsertTimer(TimerEventData* data);
	void UpdateTimer(int64_t now);
private:
	EventLoop* m_loop;

	int32_t m_timerfd;
	EpollData* m_epoll_data;

	TimerDataMap m_timer_map;
};

#endif