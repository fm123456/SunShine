#include "SceneService.h"

SceneProcess::SceneProcess(int32_t id) : m_process_id(id)
{

}

SceneProcess::~SceneProcess()
{

}

bool SceneProcess::Init()
{
	return true;
}

void SceneProcess::ProcessMsg(int32_t source_type, int32_t source_id, int32_t cmd, const std::string& msg)
{
	// process client or other server msg
}

void SceneProcess::TimerCall(int64_t now_tm)
{

}