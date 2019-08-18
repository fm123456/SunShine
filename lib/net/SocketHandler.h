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

	void Append(const char* src, size_t len)
	{
		m_write_buffer.Append(src, len);
	}

	template<class T>
	bool ReadMsg(T& header, char** src1, size_t& size1, char** src2, size_t& size2)
	{
		return m_read_buffer.ReadMsg<T>(header, src1, size1, src2, size2);
	}

	size_t GetWriteBufferUsedSize() { return m_write_buffer.GetOccupySize(); }
    void FetchReadBuffer(size_t sz) { m_read_buffer.FetchSize(sz); }
private:
	int32_t m_socket;

	CycleQueueBuffer<32> m_read_buffer;
	CycleQueueBuffer<32> m_write_buffer;
};

#endif