#include <pthread.h>
#include <stdint.h>

#include "CommonDef.h"
#include "IProcess.h"
#include "Log.h"
#include "Server.h"

class MyProcess : public IProcess
{
public:
	MyProcess(){}
	~MyProcess(){}

	void ProcessMsg(int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg) override
	{
		LOG_INFO("MyProcess ProcessMsg source_type[%d] source_id[%d] cmd[%d] msg:[%s]", source_type, source_id, cmd, msg.c_str());
	}

	void TimerCall(int64_t now_tm) override
	{
		LOG_INFO("MyProcess TimerCall now_tm[%ld]", now_tm);
	}
};

class MyProcessFactory : public IProcessFactory
{
public:
	MyProcessFactory(){}
	~MyProcessFactory(){}

	IProcessPtr CreateProcess()
	{
		return IProcessPtr(new MyProcess());
	}
};


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		LOG_DEBUG("paramter is less");
		return 0;
	}
	daemon(1, 0);
	int32_t type = atoi(argv[1]);
	IProcessFactoryPtr factory = IProcessFactoryPtr(new MyProcessFactory());
	
	if (!Server::instance().Init(type, factory))
	{
		LOG_ERROR("Server Init Failed");
		return -1;
	}
	Server::instance().Loop();
} 