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

	int32_t ReadScoket(std::vector<std::string>& msg_list);
	int32_t WriteSocket();

	void SendMessage(const std::string& str);
private:

private:
	int32_t m_socket;

	CycleQueueBuffer m_read_buffer;
	CycleQueueBuffer m_write_buffer;
};

#endif