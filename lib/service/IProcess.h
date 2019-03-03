#ifndef IPROCESS_H_
#define IPROCESS_H_

#include <stdint.h>
#include <memory>

class IProcess
{
public:
	virtual bool Init() = 0;
	virtual void ProcessMsg(int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg) = 0;
	virtual void TimerCall(int64_t now_tm){};
};
typedef std::shared_ptr<IProcess> IProcessPtr;

class IProcessFactory
{
public:
	virtual IProcessPtr CreateProcess(int32_t id) = 0;
};
typedef std::shared_ptr<IProcessFactory> IProcessFactoryPtr;

#endif