#include <string.h>

#include "EventLoop.h"
#include "Channel.h"
#include "WakeupMainProcess.h"
#include "Log.h"
#include "MessageQueue.h"
#include "KbrokerConnection.h"
#include "ServerProxy.h"


WakeupProcess::WakeupProcess()
{
}

WakeupProcess::~WakeupProcess()
{

}

bool WakeupProcess::Init(EventLoop* loop, int32_t wake_up)
{
	m_wake_up = wake_up;

	m_channel = ChannelPtr(new Channel(m_wake_up, loop));
	m_channel->SetReadCallback(std::bind(&WakeupProcess::ReadHandler, this));
	m_channel->EnableRead();

	return true;
}

void WakeupProcess::NotifyMsgArrived()
{
	//new message arrived
	static char buff[10] = "R";
	int32_t write_size = ::write(m_wake_up, buff, 1);
	if (write_size <= 0)
	{
		LOG_ERROR("WakeupProcess::NotifyWakeupSend notify error err:%d err_msg:%d", errno, strerror(errno));
	}
}

void WakeupProcess::ReadHandler()
{
	//message need to send
	char buff[1024] = { 0 };
	int32_t read_size = ::read(m_wake_up, buff, sizeof(buff));
	if (read_size > 0)
	{
		//to send
		std::string msg;
		KbrokerConnection* conn = ServerProxy::instance().GetKbrokerConnect();
		while (SendMsgQueue::instance().PopMessage(msg))
		{
			conn->SendMsg(msg);
		}
	}
	else
	{
		LOG_FATAL("WakeupProcess ReadHandler read wake up fd failed read_size:%d err:%d err_msg:%s", read_size, errno, strerror(errno));
	}
}