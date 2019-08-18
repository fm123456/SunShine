#ifndef CONNECT_MANAGER_H_
#define CONNECT_MANAGER_H_

#include <map>
#include <stdint.h>
#include "common/Singleton.h"


class UserConnection;
class ConnectManager : public Singleton<ConnectManager>
{
public:
	~ConnectManager();

	void InsertUserConnection(int32_t fd, UserConnection* conn);

	void DeleteUserConnection(int32_t fd);

private:
	std::map<int32_t, UserConnection*> m_user_conn_map;
};

#endif
