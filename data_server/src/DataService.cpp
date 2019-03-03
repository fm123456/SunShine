#include "DataService.h"

DataProcess::DataProcess(int32_t id) : m_process_id(id)
{

}

DataProcess::~DataProcess()
{

}

bool DataProcess::Init()
{
	return true;
}

void DataProcess::ProcessMsg(int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg)
{
	// process client or other server msg
}

void DataProcess::TimerCall(int64_t now_tm)
{

}