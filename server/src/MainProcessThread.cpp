#include <algorithm>
#include <string.h>

#include "MainProcessThread.h"
#include "TimerManager.h"
#include "Log.h"
#include "MessageQueue.h"
#include "Util.h"
#include "DateTime.h"



class CommonTimerTask : public ITimerTask
{
public:
	CommonTimerTask(const Task& task, int32_t cur_pid)
		:m_task(task), m_cur_pid(cur_pid)
	{
	}

	~CommonTimerTask()
	{
	}

	virtual void Execute() override
	{
		IProcessPtr cur_process = MainProcessThread::instance().GetProcess(m_cur_pid);
		if (!cur_process)
			return;
		MainProcessThread::instance().SetCurProcess(m_cur_pid, cur_process);
		m_task();
		MainProcessThread::instance().DoExpandTask();
	}

private:
	Task m_task;
	int32_t m_cur_pid;
};


class TimerCallTask : public ITimerTask
{
public:
	TimerCallTask()
	{
	}
	~TimerCallTask()
	{
	}

	virtual void Execute() override
	{
		int64_t now_tm = DateTime::NowMilliSecond();
		std::map<int32_t, IProcessPtr>& process_map = MainProcessThread::instance().GetProcessMap();
		for (auto it = process_map.begin(); it != process_map.end();it++)
		{
			MainProcessThread::instance().SetCurProcess(it->first, it->second);
			it->second->TimerCall(now_tm);
			MainProcessThread::instance().DoExpandTask();
		}
	}
};

MainProcessThread::MainProcessThread()
{
	m_exist = false;
	m_timer_event = NULL;
	m_cur_process_id = 0;
	m_cur_process = IProcessPtr();
	m_process_factory = IProcessFactoryPtr();
}

MainProcessThread::~MainProcessThread()
{
	if (m_workthread.joinable())
		m_workthread.join();
	SAFE_DELETE(m_timer_event);
}

bool MainProcessThread::Start(int32_t wake_up_send, IProcessFactoryPtr process_factory)
{
	m_timer_event = new TimerManager();
	m_process_factory = process_factory;
	m_wake_up_send = wake_up_send;
	m_workthread = std::thread(&MainProcessThread::WorkThread, this);

	return m_process_factory != IProcessFactoryPtr();
}

void MainProcessThread::NotifyWakeupSend()
{
	static char buff[10] = "S";
	int32_t write_size = ::write(m_wake_up_send, buff, 1);
	if (write_size <= 0)
	{
		LOG_ERROR("MainProcessThread::NotifyWakeupSend notify error err:%d err_msg:%d",errno, strerror(errno));
	}
}

void MainProcessThread::AddMsDelayEvent(int32_t ms, Task task)
{
	m_timer_event->AddMsDelayEvent(ms,ITimerTaskPtr(new CommonTimerTask(task, GetCurProcessId())));
}

IProcessPtr MainProcessThread::GetProcess(int32_t pid)
{
	auto it = m_process_map.find(pid);
	if (it != m_process_map.end())
	{
		return it->second;
	}
	return IProcessPtr();
}

void MainProcessThread::AddAsynEvent(Task task)
{
	m_expand_task.push_back(task);
}

void MainProcessThread::DoExpandTask()
{
	for (auto it : m_expand_task)
	{
		it();
	}
	m_expand_task.clear();
}

void MainProcessThread::WorkThread()
{
	fd_set read_fds;
	struct timeval tv;
	tv.tv_sec = 10;

	while (!MainProcessThread::instance().Exist())
	{
		FD_ZERO(&read_fds);
		FD_SET(m_wake_up_send,&read_fds);
		tv.tv_usec = std::min(m_timer_event->GetNearestMsDelay() * 1000, default_timeout_ms*1000);

		int32_t ret = ::select(m_wake_up_send + 1, &read_fds, NULL, NULL, &tv);
		if (ret < 0)
		{
			LOG_FATAL("MainProcessThread select error:%d err_string:%s",errno, strerror(errno));
			break;
		}
		if (ret == 0)
		{
			m_timer_event->ExecuteTimerEvent();
		}
		else
		{
			//msg to process
			char buff[1024] = { 0 };
			int32_t read_size = ::read(m_wake_up_send, buff, sizeof(buff));
			if (read_size > 0)
			{
				//to do message;
				CommonDataInfo info;
				while (ReceiveMsgQueue::instance().PopMessage(info))
				{
					DoProcessMsg(info.m_source_pid, info.m_dest_type, info.m_dest_pid, info.m_cmd, info.m_msg);
				}
			}
			else
			{
				LOG_FATAL("MainProcessThread read wake up fd failed read_size:%d err:%d err_msg:%s", read_size, errno, strerror(errno));
			}
		}
	}
}

void MainProcessThread::DoProcessMsg(int32_t pid, int32_t source_type, int32_t source_pid, int32_t cmd, const std::string& msg)
{
	IProcessPtr process = IProcessPtr();
	auto it = m_process_map.find(pid);
	if (it == m_process_map.end())
	{
		process = m_process_factory->CreateProcess();
		m_process_map[pid] = process;
	}
	else
	{
		process = it->second;
	}
	SetCurProcess(pid, process);

	process->ProcessMsg(source_type, source_pid, cmd, msg);
	DoExpandTask();
}