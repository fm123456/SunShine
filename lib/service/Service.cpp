#include "service/Service.h"
#include "service/ProcessManager.h"
#include "service/GatewayConnection.h"
#include "service/ServiceConnectManager.h"

#include "net/EventLoop.h"
#include "common/Log.h"


static int32_t g_service_type = 0;

Service::Service()
{

}

Service::~Service()
{

}

int32_t Service::ServiceType()
{
	return g_service_type;
}

void Service::SendMessage(int32_t type, int32_t process_id, int32_t cmd, const std::string& msg)
{

}

void Service::Start(IProcessFactoryPtr process_factory, int32_t service_type)
{
	g_service_type = service_type;
	ProcessManager::instance().SetProcessFactory(process_factory);
}

void Service::Loop()
{
	EventLoop ev;
	if (!ev.Init())
	{
		LOG_FATAL("Event loop init failed");
		return;
	}
	if (!ServiceConnectManager::instance().Init(&ev))
	{
		LOG_FATAL("ServiceConnectManager init failed");
		return;
	}
	ev.Loop();
}
