#include "ConnectManager.h"
#include "UserConnection.h"
#include "ServerConnection.h"
#include "common/Util.h"


ConnectManager::~ConnectManager()
{
	for (auto it : m_user_conn_map)
	{
		SAFE_DELETE(it.second);
	}
	for (auto it : m_server_conn_map)
	{
		SAFE_DELETE(it.second);
	}
}

void ConnectManager::InsertUserConnection(int32_t fd, UserConnection* conn)
{
	m_user_conn_map[fd] = conn;
}

void ConnectManager::DeleteUserConnection(int32_t fd)
{
	auto it = m_user_conn_map.find(fd);
	if (it != m_user_conn_map.end())
	{
		SAFE_DELETE(it->second);
	}
}

void ConnectManager::InsertServerConnection(int32_t fd, ServerConnection* conn)
{
	m_server_conn_map[fd] = conn;
}

void ConnectManager::DeleteServerConnection(int32_t fd)
{
	auto it = m_server_conn_map.find(fd);
	if (it != m_server_conn_map.end())
	{
		SAFE_DELETE(it->second);
	}
}