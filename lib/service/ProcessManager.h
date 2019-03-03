#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

#include <map>
#include "common/Singleton.h"
#include "service/IProcess.h"

class ProcessManager : public Singleton<ProcessManager>
{
public:
	ProcessManager();
	~ProcessManager();

	void SetProcessFactory(IProcessFactoryPtr process_factory);

	void ProcessServerMessage(int32_t process_id, int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg);

	int32_t GetCurProcessId() { return m_cur_process_id; }

private:
	IProcessFactoryPtr m_process_factory;
	std::map<int32_t, IProcessPtr> m_process_map;

	int32_t m_cur_process_id;
};


#endif
