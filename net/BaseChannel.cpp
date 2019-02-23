#include "BaseChannel.h"
#include "EventLoop.h"
#include "EpollData.h"
#include "SocketHandler.h"

#include "common/Util.h"


BaseChannel::BaseChannel(int32_t fd, EventLoop* loop)
	:m_fd(fd), m_loop(loop)
{
	m_handler = new SocketHandler(m_fd);
	m_epoll_data = new EpollData(m_fd, m_loop);

	m_epoll_data->SetReadCallback(std::bind(&BaseChannel::OnRead, this));
	m_epoll_data->SetWriteCallback(std::bind(&BaseChannel::OnWrite, this));
	m_epoll_data->EnableRead();
}

BaseChannel::~BaseChannel()
{
	m_epoll_data->Remove();

	SAFE_DELETE(m_handler);
	SAFE_DELETE(m_epoll_data);
}

void BaseChannel::EnableWrite()
{
	m_epoll_data->EnableWrite();
}

void BaseChannel::DisableWrite()
{
	m_epoll_data->DisableWrite();
}

void BaseChannel::EnableRead()
{
	m_epoll_data->EnableRead();
}

void BaseChannel::DisableRead()
{
	m_epoll_data->DisableRead();
}