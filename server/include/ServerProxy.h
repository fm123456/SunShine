#ifndef SERVER_PROXY_H__
#define SERVER_PROXY_H__

#include "IProcess.h"
#include "Singleton.h"

class EventLoop;
class KbrokerConnection;

class ServerProxy : public Singleton<ServerProxy>
{
public:
	ServerProxy();
	~ServerProxy();

	bool Init(int32_t type, IProcessFactoryPtr process_factory);

	void Loop();
	
	EventLoop* GetEventLoop(){ return m_loop; }
	KbrokerConnection* GetKbrokerConnect(){ return m_conn; }

	int32_t GetType(){ return m_type; }
private:
	int32_t m_type;

	EventLoop* m_loop;
	KbrokerConnection* m_conn;

	bool m_init_status;
};

#endif