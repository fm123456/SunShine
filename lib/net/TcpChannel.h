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
		EnableWrite();
		m_socket_handler->Append(msg);
	}

	void SendMsg(const char* src, size_t len)
	{
		EnableWrite();
		m_socket_handler->Append(src, len);
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
        LOG_INFO("TcpChannel OnRead");

        int32_t status = ReadSuccess;
        do 
        {
		    status = m_socket_handler->ReadScoket();
		    if (status == ReadFailed)
		    {
			    OnClose();
			    break;
		    }
		    HEADER header;
            char* src1 = NULL;
            char* src2 = NULL;
            size_t size1 = 0, size2 = 0; 
		    while (m_socket_handler->ReadMsg<HEADER>(header, &src1, size1, &src2, size2))
		    {
			    OnMessageArrived(header, src1, size1, src2, size2);
                m_socket_handler->FetchReadBuffer(sizeof(HEADER) + header.m_len);
			    //msg.clear();
		    }
        } while (status == ReadContinue);
	}

	virtual void OnWrite() override
	{
		int32_t status = m_socket_handler->WriteSocket();
		if (status == WriteFailed)
		{
			OnClose();
			return;
		}
		if (m_socket_handler->GetWriteBufferUsedSize() == 0)
		{
			DisableWrite();
		}
	}

	virtual void DoSocketClose() {}
	virtual void OnMessageArrived(const HEADER& header, char* src1, size_t size1, char* src2, size_t size2) = 0;
private:
	SocketHandler* m_socket_handler;
};

#endif
