#ifndef SOCKET_HANDLER_H__
#define SOCKET_HANDLER_H__

#include <stdint.h>
#include <list>
#include <string>
#include <functional>
#include <string>
#include <vector>

#include "common/CycleQueueBuffer.h"


enum ResultStatus
{
	ReadSuccess,
	ReadFailed,
	ReadContinue,

	WriteSuccess,
	WriteFailed,
	WriteContinue
};

class SocketHandler
{
public:
	SocketHandler(int32_t fd);
	~SocketHandler();

	int32_t ReadScoket();
	int32_t WriteSocket();

	
	void Append(const std::string& msg)
	{
		m_write_buffer.Append(msg);
	}

	template<class T>
	void Append(T& val)
	{
		m_write_buffer.Append<T>(val);
	}

	void Append(const char* src, size_t len)
	{
		m_write_buffer.Append(src, len);
	}

	template<class T>
	bool ReadMsg(T& header, std::string& msg)
	{
		return m_read_buffer.ReadMsg<T>(header, msg);
	}

	size_t GetWirteBufferSize() { return m_write_buffer.Size(); }
	size_t GetReadBufferSize() { return m_read_buffer.Size(); }
private:
	int32_t m_socket;

	CycleQueueBuffer m_read_buffer;
	CycleQueueBuffer m_write_buffer;
};

#endif