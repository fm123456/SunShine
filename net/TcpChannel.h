#ifndef TCP_CHANNEL_H_
#define TCP_CHANNEL_H_

#include <string>

#include "BaseChannel.h"

#include "net/SocketHandler.h"
#include "common/Log.h"
#include "common/Util.h"

template<class HEADER>
class TcpChannel : public BaseChannel
{
public:
	TcpChannel(int32_t fd, EventLoop* loop)
		:BaseChannel(fd, loop)
	{
		m_socket_handler = new SocketHandler(fd);
	}

	virtual ~TcpChannel()
	{
		SAFE_DELETE(m_socket_handler);
	}

	void SendMsg(const std::string& msg)
	{
		CheckEnableWrite();
		m_socket_handler->Append(msg);
	}
	
	template<class T>
	void SendMsg(T& val)
	{
		CheckEnableWrite();
		m_socket_handler->Append<T>(val);
	}

	void SendMsg(const char* src, size_t len)
	{
		CheckEnableWrite();
		m_socket_handler->Append(src, len);
	}

private:
	void CheckEnableWrite()
	{
		if (m_socket_handler->GetWirteBufferSize() > 0)
			return;
		EnableWrite();
	}

private:
	void OnClose()
	{
		BaseChannel::Remove();
		DoSocketClose();
	}

private:
	virtual void OnRead() override
	{
		int32_t status = m_socket_handler->ReadScoket();
		if (status == ReadFailed)
		{
			OnClose();
			return;
		}
		HEADER header;
		std::string msg;
		while (m_socket_handler->ReadMsg<HEADER>(header, msg))
		{
			OnMessageArrived(header, msg);
			msg.clear();
		}
	}

	virtual void OnWrite() override
	{
		int32_t status = m_socket_handler->WriteSocket();
		if (status == WriteFailed)
		{
			OnClose();
			return;
		}
		if (m_socket_handler->GetWirteBufferSize() == 0)
		{
			DisableWrite();
		}
	}

	virtual void DoSocketClose() {}
	virtual void OnMessageArrived(const HEADER& header, const std::string& msg) = 0;
private:
	SocketHandler* m_socket_handler;
};

#endif
