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
	int32_t m_server_type;
	int32_t m_process_id;
	int32_t m_cmd;
	int32_t m_len;
};

#endif
