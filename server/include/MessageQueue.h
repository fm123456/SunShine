#ifndef MESSAGE_QUEUE_H__
#define MESSAGE_QUEUE_H__

#include "Singleton.h"
#include "NoneLockQueue.h"
#include "data_struct/KbrokeDataStruct.h"
#include "MainProcessThread.h"
#include "WakeupMainProcess.h"

/* IO线程与逻辑线程之间的消息队列结构
*/

class ReceiveMsgQueue : public Singleton<ReceiveMsgQueue>
{
public:
	ReceiveMsgQueue(){};
	~ReceiveMsgQueue(){}

	void PushMessage(const std::string& msg)
	{
		m_receive_queue.PushBack(msg);
		WakeupProcess::instance().NotifyMsgArrived();
	}

	bool PopMessage(CommonDataInfo& info)
	{
		std::string msg;
		if (m_receive_queue.PopFront(msg))
		{
			info.FromString(msg);
			return true;
		}
		return false;
	}
private:
	NoneLockQueue<std::string> m_receive_queue;
};


class SendMsgQueue : public Singleton<SendMsgQueue>
{
public:
	SendMsgQueue(){}
	~SendMsgQueue(){}

	void PushMessage(int32_t dest_type, int32_t dest_pid, int32_t cmd, const std::string& msg)
	{
		CommonDataInfo info(MainProcessThread::instance().GetCurProcessId(), dest_type, dest_pid, cmd, msg);
		std::string send_msg;
		info.ToString(send_msg);
		m_send_queue.PushBack(send_msg);

		MainProcessThread::instance().NotifyWakeupSend();
	}

	bool PopMessage(std::string& msg)
	{
		return m_send_queue.PopFront(msg);
	}
private:
	NoneLockQueue<std::string> m_send_queue;
};

#endif