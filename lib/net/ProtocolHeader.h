#ifndef PROTOCOL_HEADER_H_
#define PROTOCOL_HEADER_H_

/* �ͻ��˰�ͷ
*/
struct ClientHeader
{
	int32_t m_cmd;
	int32_t m_len;
};


/* ������֮��İ�ͷ
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
