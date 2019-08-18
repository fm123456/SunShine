#include "UserConnection.h"
#include "ConnectManager.h"
#include "common/Log.h"
#include "proto/Common.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>


UserConnection::UserConnection(int32_t fd, EventLoop* loop, const std::string& addr, int32_t port)
	:TcpChannel<ClientHeader>(fd, loop),m_addr(addr),m_port(port)
{

}

UserConnection::~UserConnection()
{

}

void UserConnection::DoSocketClose()
{
	LOG_INFO("UserConnection::DoSocketClose");
	ConnectManager::instance().DeleteUserConnection(GetFd());
}

void UserConnection::OnMessageArrived(const ClientHeader& header, char* src1, size_t size1, char* src2, size_t size2)
{
    SSProto::TestChat tsChat;

    if (src2 && size2 > 0)
    {
        void* data[2];
        data[0] = src1;
        data[1] = src2;

        int size[2];
        size[0] = (int)size1;
        size[1] = (int)size2;

        google::protobuf::io::MultiArrayInputStream input_stream((const void**)data, (int*)size, 2);
        tsChat.ParseFromZeroCopyStream(&input_stream);
    }
    else
    {
        tsChat.ParseFromArray(src1, size1);
    }

    sleep(2);
    LOG_INFO("UserConnection::OnMessageArrived cmd[%d] size1[%d] size2[%d] msg[%s]", header.m_cmd, size1, size2, tsChat.content().c_str());
	//SendMsg((char*)&header, sizeof(ClientHeader));
	//SendMsg(msg);
}