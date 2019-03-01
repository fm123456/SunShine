#ifndef EPOLL_DATA_H__
#define EPOLL_DATA_H__

#include <stdint.h>
#include <memory>
#include "net/CommonDef.h"

//epoll�д�����ļ���������Ӧ�Ķ��󣬲������Ӧ���ļ�������
//�������ļ����������������


enum ChannelStatus
{
	IDLE_STATUS = 0,
	RUNNING_STATUS = 1,
	DELETING_STATUS = 2,
};

class EventLoop;
class EpollData 
{
public:
	EpollData(int32_t fd, EventLoop* loop);
	~EpollData();

	void EnableWrite();
	void DisableWrite();
	void EnableRead();
	void DisableRead();

	int32_t GetFd(){return m_fd;}
	int32_t GetStatus(){return m_cur_status;}
	int32_t GetEvents(){return m_events;}

	void Remove();

	void SetWriteCallback(WriteCallbackFunc func ){m_write_callback = func;}
	void SetReadCallback(ReadCallbackFunc func){m_read_callback = func;}

	void HandleEvent(int32_t epoll_event);
private:
	void Update();
private:
	int32_t m_fd;
	EventLoop* m_loop;

	//��ǰfd��ע���¼�
	int32_t m_events; 

	/*��ǰfd��״̬ 0����ʼ�� 1�����¼�ѭ���� 2���ȴ��Ƴ��¼� �ֱ��Ӧepoll_ctl�е�ADD��MODIFY��DEL*/
	int32_t m_cur_status; 


	WriteCallbackFunc m_write_callback;
	ReadCallbackFunc m_read_callback;
};

#endif