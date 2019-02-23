#include "Server.h"
#include "ServerProxy.h"
#include "MainProcessThread.h"
#include "MessageQueue.h"
#include "ServerType.h"

Server::Server()
{

}

Server::~Server()
{

}

bool Server::Init(int32_t type, IProcessFactoryPtr process_factory)
{
	return ServerProxy::instance().Init(type, process_factory);
}

void Server::AddMsDelayEvent(int32_t ms, Task task)
{
	MainProcessThread::instance().AddMsDelayEvent(ms, task);
}

void Server::AddAsynEvent(Task task)
{
	MainProcessThread::instance().AddAsynEvent(task);
}

void Server::Loop()
{
	ServerProxy::instance().Loop();
}

void Server::SendClientMsg(int32_t uid, int32_t cmd, const std::string& msg)
{
	SendMsgQueue::instance().PushMessage(GATEWAY_TYPE, uid, cmd, msg);
}

void Server::SendServerMsg(int32_t type, int32_t pid, int32_t cmd, const std::string& msg)
{
	SendMsgQueue::instance().PushMessage(type, pid, cmd, msg);
}