#include <unistd.h>
#include <string.h>

#include "EventLoop.h"
#include "ServerProxy.h"
#include "CommonDef.h"
#include "KbrokerConnection.h"
#include "WakeupMainProcess.h"
#include "MainProcessThread.h"
#include "Log.h"

ServerProxy::ServerProxy()
{
	m_loop = NULL;
	m_conn = NULL;

	m_init_status = false;
}

ServerProxy::~ServerProxy()
{
	SAFE_DELETE(m_loop);
	SAFE_DELETE(m_conn);
}

bool ServerProxy::Init(int32_t type, IProcessFactoryPtr process_factory)
{
	m_type = type;

	m_loop = new EventLoop();
	if (!m_loop->Init())
	{
		LOG_ERROR("ServerProxy Event loop init failed");
		return false;
	}
	m_conn = new KbrokerConnection(m_loop);
	if (!m_conn->Init())
	{
		LOG_ERROR("ServerProxy KbrokerConnection init failed");
		return false;
	}

	int32_t pip[2];
	int32_t ret = pipe(pip);
	if (ret != 0)
	{
		LOG_ERROR("ServerProxy create pip failed err:%d err_msg:%s", errno, strerror(errno));
		return false;
	}

	if (!WakeupProcess::instance().Init(m_loop, pip[0]))
	{
		LOG_ERROR("ServerProxy WakeupProcess init failed");
		return false;
	}
	if (!MainProcessThread::instance().Start(pip[1], process_factory))
	{
		LOG_ERROR("ServerProxy MainProcessThread init failed");
		return false;
	}
	m_init_status = true;
	return true;
}

void ServerProxy::Loop()
{
	if (!m_init_status)
	{
		LOG_INFO("ServerProxy EventLoop init failed");
		return;
	}
	m_loop->Loop();
}