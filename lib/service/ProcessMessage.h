#ifndef PROCESS_MESSAGE_H_
#define PROCESS_MESSAGE_H_

#include <string>
#include "net/ProtocolHeader.h"

class ProcessMessage
{
public:
	static void SendMessageToGateway(int32_t fd, int32_t target_type, int32_t target_id, int32_t cmd, const std::string& msg);
	static void ReceiveMessageFromGateway(const ServerHeader& header, const std::string& msg);

	static void SendMessageToMaster(int32_t fd, int32_t cmd, const std::string& msg);
	static void ReceiveMessageFromMaster(const MasterHeader& header, const std::string& msg);
};


#endif
