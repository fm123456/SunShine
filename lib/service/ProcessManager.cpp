#include "service/ProcessManager.h"

#include "common/Log.h"

ProcessManager::ProcessManager()
{
	m_process_factory = IProcessFactoryPtr();
}

ProcessManager::~ProcessManager()
{

}

void ProcessManager::SetProcessFactory(IProcessFactoryPtr process_factory)
{
	m_process_factory = process_factory;
}

void ProcessManager::ProcessServerMessage(int32_t process_id, int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg)
{
	auto it = m_process_map.find(process_id);
	if (it == m_process_map.end())
	{
		LOG_WARM("ProcessManager::ProcessServerMessage process id not found process_id[%d]", process_id);
		return;
	}
	m_cur_process_id = process_id;
	it->second->ProcessMsg(source_type, source_id, cmd, msg);
	m_cur_process_id = 0;
}