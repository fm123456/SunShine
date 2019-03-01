#ifndef PROTOCOL_HEADER_H_
#define PROTOCOL_HEADER_H_

/* 客户端包头
*/
struct ClientHeader
{
	int32_t m_cmd;
	int32_t m_len;
};


/* 服务器之间的包头
*/
struct ServerHeader
{
	int32_t m_src_server_type;
	int32_t m_src_process_id;
	int32_t m_target_server_type;
	int32_t m_target_process_id;
	int32_t m_cmd;
	int32_t m_len;
};

#endif
