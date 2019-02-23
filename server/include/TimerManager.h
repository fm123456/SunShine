#ifndef TIMER_MANAGER_H__
#define TIMER_MANAGER_H__

#include <stdint.h>
#include <vector>
#include <map>

#include "CommonDef.h"

typedef std::function<void(void)> AfterTaskCallback;
const static int64_t default_timeout_ms = 60 * 1000;//1∑÷÷”


class ITimerTask
{
public:
	virtual void Execute() = 0;
};
typedef std::shared_ptr<ITimerTask> ITimerTaskPtr;

class TimerInfo
{
public:
	TimerInfo(uint64_t _expiration, const ITimerTaskPtr& _task, uint32_t _interval = 0);
	~TimerInfo();

	int64_t GetExpiration(){ return m_expiration; }
	bool IsReapt(){ return m_interval != 0; }

	void Reset(int64_t now);
	void Execute();

private:
	uint64_t m_expiration;
	uint32_t m_interval;

	ITimerTaskPtr m_task;
};
typedef std::vector<TimerInfo*> TimerInfoList;
typedef std::map<int64_t, TimerInfoList> TimerInfoMap;

class TimerManager
{
public:
	TimerManager();
	~TimerManager();

	void AddMsDelayEvent(uint64_t _delay, const ITimerTaskPtr& _task, uint32_t _interval = 0);

	void ExecuteTimerEvent(int64_t now_tm = 0);

	int64_t GetNearestMsDelay(int64_t now_tm = 0);
private:
	bool InsertTimer(TimerInfo* data);

private:
	TimerInfoMap m_timer_map;
};


#endif