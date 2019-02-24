#include <iostream>

#include "StdCout.h"
#include "ClientConnection.h"
#include "ClientManager.h"

#include "common/Log.h"
#include "net/Command.h"

StdCout::StdCout(EventLoop* loop)
	:BaseChannel(2, loop)
{

}

StdCout::~StdCout()
{

}

void StdCout::OnChat(int32_t cmd)
{
	DisableRead();

	std::string msg;

	std::cout << "input send msg:";
	std::cin >> msg;

	ClientHeader header;
	header.m_cmd = cmd;
	header.m_len = msg.length();

	ClientManager::instance().GetClientConn()->SendMsg<ClientHeader>(header);
	ClientManager::instance().GetClientConn()->SendMsg(msg);

	LOG_INFO("StdCout::OnChat cmd:%d send_msg:%s", cmd, msg.c_str());

	EnableRead();
}

void StdCout::OnRead()
{
	LOG_INFO("StdCout::OnWrite");

	char buff[4096] = {0};
	int32_t read_size = ::read(2,buff,4096) - 1;
	//LOG_PRINT("StdCout ReadHandler msg:%s read_size:%d",buff, read_size);
	m_cmd = atoi(buff);
	LOG_INFO("input cmd: %d", m_cmd);
	switch (m_cmd)
	{
	case CHAT_CMD:
		OnChat(m_cmd);
		break;
	default:
		LOG_INFO("invalid cmd cmd:%d", m_cmd);
		break;
	}
}

//StdCout::StdCout(ClientConnection* client, EventLoop* loop)
//	:m_client(client),m_loop(loop),m_cmd(0)
//{
//	m_channel = ChannelPtr(new Channel(2, m_loop));
//	m_channel->SetReadCallback(std::bind(&StdCout::ReadHandler, this));
//	m_channel->SetWriteCallback(std::bind(&StdCout::WriteHandler, this));
//	m_channel->EnableRead();
//
//	m_handler = new SocketHandler(2);
//}
//
//StdCout::~StdCout()
//{
//	SAFE_DELETE(m_handler);
//}
//
//int32_t CheckNumber(const char* buff, int32_t sz)
//{
//	for (int32_t i = 0; i < sz; i++)
//	{
//		if (buff[i] >= '0' && buff[i] <= '9')
//			continue;
//		return 0;
//	}
//	return atoi(buff);
//}
//
//void StdCout::OnChat(int32_t cmd)
//{
//	int32_t user_id;
//	char msg_buff[1024] = { 0 };
//
//	std::cout << "input target user id:";
//	std::cin >> user_id;
//
//	std::cout << "input send msg:";
//	std::cin >> msg_buff;
//
//	ChatDataInfo chat;
//	chat.m_user_id = user_id;
//	chat.m_msg = std::string(msg_buff);
//	
//	std::string data_str;
//	chat.ToString(data_str);
//
//	m_client->SendMsg(cmd, data_str);
//	LOG_PRINT("StdCout::OnChat user_id:%d send_msg:%s", user_id, msg_buff);
//}
//
//void StdCout::ReadHandler()
//{
//	char buff[4096] = {0};
//	int32_t read_size = ::read(2,buff,4096) - 1;
//	//LOG_PRINT("StdCout ReadHandler msg:%s read_size:%d",buff, read_size);
//
//	if (m_cmd == 0)
//	{
//		m_cmd = CheckNumber(buff, read_size);
//		switch (m_cmd)
//		{
//		case CHAT_CMD:
//			OnChat(m_cmd);
//			break;
//		default:
//			LOG_PRINT("invalid cmd cmd:%d",m_cmd);
//			break;
//		}
//
//		m_cmd = 0;
//	}
//	if (m_cmd == 0)
//	{
//		LOG_PRINT("Input cmd:");
//	}
//}
//void StdCout::WriteHandler()
//{
//	LOG_PRINT("StdCout WriteHandler");
//}