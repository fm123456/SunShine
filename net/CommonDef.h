#ifndef COMMON_DEF_H__
#define COMMON_DEF_H__

#include <map>
#include <vector>
#include <memory>
#include <unistd.h>
#include <functional>
#include <string>
#include <list>

class EpollData;

typedef std::function<void(void)> WriteCallbackFunc;
typedef std::function<void(void)> ReadCallbackFunc;

typedef std::function<void(void)> Task;
typedef std::function<void(int32_t, int32_t, const std::string&)> ProcessMessageCallback;
typedef std::list<Task> TaskList;

typedef std::vector<std::string> MessageList;

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

#endif