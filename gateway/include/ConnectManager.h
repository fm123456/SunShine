#ifndef CONNECT_MANAGER_H_
#define CONNECT_MANAGER_H_

#include <map>
#include <stdint.h>
#include "common/Singleton.h"


class UserConnection;
class ServerConnection;
class ConnectManager : public Singleton<ConnectManager>
{
public:
	~ConnectManager();

	void InsertUserConnection(int32_t fd, UserConnection* conn);

	void DeleteUserConnection(int32_t fd);

	void InsertServerConnection(int32_t fd, ServerConnection* conn);

	void DeleteServerConnection(int32_t fd);

private:
	std::map<int32_t, UserConnection*> m_user_conn_map;
	std::map<int32_t, ServerConnection*> m_server_conn_map;
};

#endif
