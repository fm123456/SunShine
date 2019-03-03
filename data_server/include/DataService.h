#ifndef DATA_SERVICE_H_
#define DATA_SERVICE_H_

#include "service/IProcess.h"


class DataProcess : public IProcess
{
public:
	DataProcess(int32_t id);
	~DataProcess();

	virtual bool Init() override;
	virtual void ProcessMsg(int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg) override;
	virtual void TimerCall(int64_t now_tm) override ;

private:
	int32_t m_process_id;
};

class DataProcessFactory : public IProcessFactory
{
public:
	virtual IProcessPtr CreateProcess(int32_t id)
	{
		return IProcessPtr(new DataProcess(id));
	}
};


#endif
