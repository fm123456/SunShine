#ifndef SERVER_H__
#define SERVER_H__

#include "CommonDef.h"
#include "IProcess.h"
#include "Singleton.h"

class Server : public Singleton<Server>
{
public:
	Server();
	~Server();

	bool Init(int32_t type, IProcessFactoryPtr process_factory);

	void AddMsDelayEvent(int32_t ms, Task task);

	//异步事件中添加异步事件会接着在当前的异步列表后面执行，所以不能循环添加异步事件，这样会造成死循环
	void AddAsynEvent(Task task);

	void Loop();

	void SendClientMsg(int32_t uid, int32_t cmd, const std::string& msg);

	void SendServerMsg(int32_t type, int32_t pid, int32_t cmd, const std::string& msg);
};

#endif