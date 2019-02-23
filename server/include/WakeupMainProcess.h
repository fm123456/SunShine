#ifndef WAKEUP_PROCESS_THREAD_H__
#define WAKEUP_PROCESS_THREAD_H__

#include "Singleton.h"
#include "CommonDef.h"

class EventLoop;

class WakeupProcess : public Singleton<WakeupProcess>
{
public:
	WakeupProcess();
	~WakeupProcess();

	bool Init(EventLoop* loop, int32_t wake_up);

	void NotifyMsgArrived();
private:
	void ReadHandler();

private:
	int32_t m_wake_up;
	ChannelPtr m_channel;
};

#endif