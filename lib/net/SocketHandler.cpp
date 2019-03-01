#include <unistd.h>
#include <sys/uio.h>

#include "common/Log.h"

#include "CommonDef.h"
#include "SocketHandler.h"


SocketHandler::SocketHandler(int32_t fd)
	:m_socket(fd)
{

}

SocketHandler::~SocketHandler()
{

}

int32_t SocketHandler::ReadScoket()
{
	int32_t status = ReadSuccess;
	int32_t read_size = m_read_buffer.ReadFd(m_socket);
	if (read_size <= 0)
	{
		status = ReadFailed;
		if (errno == 0)
		{
			LOG_DEBUG("Socket Close fd: %d", m_socket);
		}
		else if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
		{
			LOG_DEBUG("Socket is EAGAIN or EINTR or EWOULDBLOCK err[%d]", errno);
			status = ReadContinue;
		}
		else
		{
			LOG_DEBUG("Read Socket Unknow Error err[%d]", errno);
		}
	}
	return status;

}

int32_t SocketHandler::WriteSocket()
{
	int32_t status = WriteSuccess;

	int32_t write_size = m_write_buffer.WriteFd(m_socket);
	if (write_size <= 0)
	{
		status = WriteFailed;
		if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
		{
			status = WriteContinue;
			LOG_DEBUG("WriteSocket WriteContinue");
		}
	}
	return status;
}