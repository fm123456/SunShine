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

	//�첽�¼�������첽�¼�������ڵ�ǰ���첽�б����ִ�У����Բ���ѭ������첽�¼��������������ѭ��
	void AddAsynEvent(Task task);

	void Loop();

	void SendClientMsg(int32_t uid, int32_t cmd, const std::string& msg);

	void SendServerMsg(int32_t type, int32_t pid, int32_t cmd, const std::string& msg);
};

#endif