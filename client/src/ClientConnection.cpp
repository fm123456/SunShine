#include <unistd.h>
#include <netinet/in.h>

#include "ClientConnection.h"

#include "common/Log.h"

ClientConnection::ClientConnection(int32_t client_fd, EventLoop* loop)
	:TcpChannel<ClientHeader>(client_fd, loop)
{

}

ClientConnection::~ClientConnection()
{

}

void ClientConnection::DoSocketClose()
{
	LOG_INFO("ClientConnection::DoSocketClose");
}

void ClientConnection::OnMessageArrived(const ClientHeader& header, const std::string& msg)
{
	LOG_INFO("ClientConnection::DoSocketClose cmd[%d] msg[%s]", header.m_cmd, msg.c_str());
}

//ClientConnection::ClientConnection(EventLoop* loop)
//	:m_loop(loop)
//{
//	m_socket = 0;
//	m_channel = ChannelPtr();
//	m_handler = NULL;
//}
//
//ClientConnection::~ClientConnection()
//{
//	CLOSE_FD(m_socket);
//	SAFE_DELETE(m_handler);
//}
//
//bool ClientConnection::Init(const std::string& addr, int32_t port, int32_t user_id)
//{
//	m_socket = SocketUtil::CreateSocket(false);
//	if (m_socket < 0)
//	{
//		LOG_PRINT("Create Socket Failed err:%d", errno);
//		return false;
//	}
//	struct sockaddr_in server_addr;
//	SocketUtil::BindAddr(addr, port, server_addr);
//
//	int32_t timeouts[]={1,2,4,8,16,1};
//	int32_t try_time = 0;
//	while(try_time <= 5)
//	{
//		if(::connect(m_socket, (sockaddr*)(&server_addr), sizeof(server_addr)) < 0)
//		{
//			LOG_PRINT("try connect failed try_times:%d errno:%d", try_time+1, errno);
//			sleep(timeouts[try_time]);
//			try_time++;
//			continue;
//		}
//		break;
//	}
//	if (try_time > 5)
//	{
//		LOG_PRINT("Connect Failed Now Return");
//		return false;
//	}
//
//	Util::MakeNonBlock(m_socket);
//	LOG_PRINT("connect success");
//	m_channel = ChannelPtr(new Channel(m_socket, m_loop));
//	m_channel->SetReadCallback(std::bind(&ClientConnection::ReadHandler, this));
//	m_channel->SetWriteCallback(std::bind(&ClientConnection::WriteHandler, this));
//	m_channel->EnableRead();
//
//	m_user_id = user_id;
//	m_handler = new SocketHandler(m_socket);
//	RegisterUserid();
//	return true;
//}
//
//void ClientConnection::RegisterUserid()
//{
//	LoginDataInfo login_data;
//	login_data.m_user_id = m_user_id;
//	login_data.m_test = 10001;
//	LOG_PRINT("ClientConnection::RegisterUserid usr_id[%d]", m_user_id);
//
//	std::string serialize_data;
//	login_data.ToString(serialize_data);
//
//	SendMsg(LOGIN_CHECK_USER_REQ, serialize_data);
//}
//
//void ClientConnection::SendMsg(int32_t cmd, const std::string& msg)
//{
//	CommonClientDataInfo client_data;
//	client_data.m_cmd = cmd;
//	client_data.m_msg = msg;
//
//	std::string serialize_data;
//	client_data.ToString(serialize_data);
//
//	m_handler->SendMessage(serialize_data);
//	m_channel->EnableWrite();
//}
//
//void ClientConnection::ReadHandler()
//{
//	MessageList msg_list;
//	m_handler->ReadScoket(msg_list);
//	if (msg_list.size() == 0)
//	{
//		LOG_PRINT("ClientConnection::ReadHandler msg size is null");
//		return;
//	}
//
//	
//	for (auto it = msg_list.begin(); it != msg_list.end(); it++)
//	{
//		CommonClientDataInfo comm;
//		if (!comm.FromString(*it))
//		{
//			LOG_PRINT("ClientConnection::ReadHandler Fromstring is error");
//			continue;
//		}
//
//		LOG_PRINT("ClientConnection::ReadHandler cmd:%d msg:%s", comm.m_cmd, comm.m_msg.c_str());
//	}
//
//	//LOG_PRINT("ClientConnection::ReadHandler msg: %s",msg.c_str());
//}
//void ClientConnection::WriteHandler()
//{
//	//LOG_PRINT("ClientConnection::WriteHandler");
//	int32_t status = m_handler->WriteSocket();
//	if (status == WriteSuccess)
//	{
//		m_channel->DisableWrite();
//	}
//	else if (status == WriteFailed)
//	{
//		LOG_PRINT("ClientConnection::WriteHandler Failed");
//	}
//}