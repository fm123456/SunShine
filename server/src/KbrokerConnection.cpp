#include <unistd.h>
#include <netinet/in.h>

#include "KbrokerConnection.h"
#include "EventLoop.h"
#include "CommonDef.h"
#include "SocketUtil.h"
#include "Channel.h"
#include "SocketHandler.h"
#include "Util.h"
#include "ServerType.h"

#include "Log.h"
#include "ParseConfig.h"
#include "ServerProxy.h"
#include "MessageQueue.h"
#include "data_struct/KbrokeDataStruct.h"

KbrokerConnection::KbrokerConnection(EventLoop* loop)
	:m_loop(loop)
{
	m_socket = 0;
	m_channel = ChannelPtr();
	m_handler = NULL;
}

KbrokerConnection::~KbrokerConnection()
{
	CLOSE_FD(m_socket);
	SAFE_DELETE(m_handler);
}

bool KbrokerConnection::Init()
{
	ParseConfig kbroker_parse;
	if (!kbroker_parse.Init("./conf/kbroker.conf"))
	{
		LOG_FATAL("KbrokerConnection::Init kbroker.conf error");
		return false;
	}
	std::string addr = kbroker_parse.GetValue("KBROKER_HOST");
	std::string port_str = kbroker_parse.GetValue("KBROKER_PORT");
	if (addr.empty() || port_str.empty())
	{
		LOG_FATAL("KbrokerConnection::Init paramter is empty error");
		return false;
	}
	LOG_INFO("KbrokerConnection::Init addr:%s port:%s", addr.c_str(), port_str.c_str());
	m_socket = SocketUtil::CreateSocket(false);
	if (m_socket < 0)
	{
		LOG_FATAL("Create Socket Failed err:%d", errno);
		return false;
	}
	struct sockaddr_in server_addr;
	SocketUtil::BindAddr(addr, atoi(port_str.c_str()), server_addr);

	int32_t timeouts[]={1,2,4,8,16,1};
	int32_t try_time = 0;
	while(try_time <= 5)
	{
		if(::connect(m_socket, (sockaddr*)(&server_addr), sizeof(server_addr)) < 0)
		{
			LOG_ERROR("try connect failed try_times:%d errno:%d", try_time+1, errno);
			sleep(timeouts[try_time]);
			try_time++;
			continue;
		}
		break;
	}
	if (try_time > 5)
	{
		LOG_FATAL("Connect Failed Now Return");
		return false;
	}

	Util::MakeNonBlock(m_socket);
	LOG_DEBUG("connect success");
	m_channel = ChannelPtr(new Channel(m_socket, m_loop));
	m_channel->SetReadCallback(std::bind(&KbrokerConnection::ReadHandler, this));
	m_channel->SetWriteCallback(std::bind(&KbrokerConnection::WriteHandler, this));
	m_channel->EnableRead();

	m_handler = new SocketHandler(m_socket);
	RegisterType();
	return true;
}

void KbrokerConnection::RegisterType()
{
	RegisterDataInfo reg;
	reg.m_type = ServerProxy::instance().GetType();

	std::string serialize_data;
	reg.ToString(serialize_data);
	SendMsg(serialize_data);
}

void KbrokerConnection::SendMsg(const std::string& str)
{
	m_handler->SendMessage(str);
	m_channel->EnableWrite();
}

void KbrokerConnection::ReadHandler()
{
	MessageList msg_list;
	int32_t status = m_handler->ReadScoket(msg_list);
	if (ReadFailed == status)
	{
		LOG_DEBUG("KbrokerConnection::ReadHandler Failed");
		return;
	}

	LOG_DEBUG("KbrokerConnection::ReadHandler msg size: %d",msg_list.size());
	for (auto it = msg_list.begin(); it != msg_list.end(); it++)
	{
		ReceiveMsgQueue::instance().PushMessage(*it);
	}

	//LOG_DEBUG("KbrokerConnection::ReadHandler msg: %s",msg.c_str());
}
void KbrokerConnection::WriteHandler()
{
	//LOG_DEBUG("KbrokerConnection::WriteHandler");
	int32_t status = m_handler->WriteSocket();
	if (status == WriteSuccess)
	{
		m_channel->DisableWrite();
	}
	else if (status == WriteFailed)
	{
		LOG_DEBUG("KbrokerConnection::WriteHandler Failed");
	}
}