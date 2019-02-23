#ifndef CLIENT_MANAGER_H_
#define CLIENT_MANAGER_H_

#include "common/Singleton.h"
#include "common/Util.h"
#include "net/EventLoop.h"

#include "ClientConnection.h"
#include "StdCout.h"

class ClientManager : public Singleton<ClientManager>
{
public:
	ClientManager()
	{

	}

	~ClientManager()
	{
		SAFE_DELETE(m_client_conn);
		SAFE_DELETE(m_stdcout);
	}

	void Init(int32_t client_fd, EventLoop* loop)
	{
		m_client_conn = new ClientConnection(client_fd, loop);
		m_stdcout = new StdCout(loop);
	}

	ClientConnection* GetClientConn() { return m_client_conn; }
	StdCout* GetStdCout() { return m_stdcout; }

private:
	ClientConnection* m_client_conn = NULL;
	StdCout* m_stdcout = NULL;


};


#endif
