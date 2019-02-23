#ifndef EPOLL_DATA_H__
#define EPOLL_DATA_H__

#include <stdint.h>
#include <memory>
#include "net/CommonDef.h"

//epoll中传入的文件描述符对应的对象，不管理对应的文件描述符
//更像是文件描述符对外的载体


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

	//当前fd关注的事件
	int32_t m_events; 

	/*当前fd的状态 0：初始化 1：在事件循环中 2：等待移除事件 分别对应epoll_ctl中的ADD、MODIFY、DEL*/
	int32_t m_cur_status; 


	WriteCallbackFunc m_write_callback;
	ReadCallbackFunc m_read_callback;
};

#endif