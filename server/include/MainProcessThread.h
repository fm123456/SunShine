#ifndef MAIN_PROCESS_THREAD_H__
#define MAIN_PROCESS_THREAD_H__

#include <thread>
#include <atomic>
#include <map>
#include "IProcess.h"
#include "Singleton.h"
#include "CommonDef.h"


class TimerManager;
class MainProcessThread : public Singleton<MainProcessThread>
{
public:
	MainProcessThread();
	~MainProcessThread();

	bool Start(int32_t wake_up_send, IProcessFactoryPtr process_factory);

	void NotifyWakeupSend();

	void AddMsDelayEvent(int32_t ms, Task task);

	void AddAsynEvent(Task task);

	void DoExpandTask();

	IProcessPtr GetProcess(int32_t pid);

	void SetExist(){ m_exist.store(true, std::memory_order_relaxed); }
	bool Exist(){ return m_exist.load(std::memory_order_relaxed); }

	int32_t GetCurProcessId(){ return m_cur_process_id; }
	void SetCurProcess(int32_t pid, IProcessPtr process){ m_cur_process_id = pid; m_cur_process = process; }

	std::map<int32_t, IProcessPtr>& GetProcessMap(){ return m_process_map; }
private:
	void WorkThread();

	void DoProcessMsg(int32_t pid, int32_t source_type, int32_t source_pid, int32_t cmd, const std::string& msg);
private:
	//唤醒io线程有数据需要发送
	int32_t m_wake_up_send;

	std::atomic<bool> m_exist;

	//正在业务主线程中使用
	TimerManager* m_timer_event;
	TaskList m_expand_task;
	std::thread m_workthread;

	std::map<int32_t, IProcessPtr> m_process_map;
	IProcessFactoryPtr m_process_factory;
	int32_t m_cur_process_id;
	IProcessPtr m_cur_process;

};

#endif