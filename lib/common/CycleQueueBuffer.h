#ifndef CYCLE_QUEUE_BUFFER_H__
#define CYCLE_QUEUE_BUFFER_H__

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/uio.h>
#include <cmath>
#include <vector>

#include "Util.h"

#define IS_POWER_OF_2(x) (!((x)&((x)-1)))

#define M1 1048576 //1024*1204

//size must be the pow of 2
template<size_t sz = M1>
class CycleQueueBuffer
{
public:
	CycleQueueBuffer()
		:m_buffer(NULL),m_size(sz),m_head(0),m_tail(0)
	{
		m_head = 0;
		m_tail = 0;

        m_buffer = new char[sz];
	}
	~CycleQueueBuffer()
	{
        SAFE_DELETE_ARRAY(m_buffer);
	}

	void Append(const std::string& msg)
	{
		if (msg.empty())
			return;
		Append(&msg[0], msg.length());
	}

	void Append(const char* src, size_t len)
	{
		if (len == 0) return;
		if (len > GetFreeSize())
		{
            //have no enough room
            return;
		}
		if (m_head > m_tail)
		{
			memcpy(&m_buffer[m_tail], &src[0], len);
			m_tail += len;
		}
		else
		{
			size_t tail = GetIndex(m_tail + len);
			if (tail > m_tail)
			{
				memcpy(&m_buffer[m_tail], &src[0], len);
			}
			else
			{
				memcpy(&m_buffer[m_tail], &src[0], len - tail);
				memcpy(&m_buffer[0], &src[len - tail], tail);
			}
			m_tail = tail;
		}
	}

	template<class HEADER>
	bool ReadMsg(HEADER& header, char** src1, size_t& size1, char** src2, size_t& size2)
	{
		if (!ReadBuffer<HEADER>(header))
			return false;
        if (header.m_len + sizeof(HEADER) > GetOccupySize())
            return false;

        size_t tmp_header = GetIndex(m_head + sizeof(header));
        if (m_size - tmp_header >= header.m_len)
        {
            *src1 = (m_buffer + tmp_header);
            size1 = header.m_len;
            *src2 = NULL;
            size2 = 0;
        }
        else
        {
            *src1 = (m_buffer + tmp_header);
            size1 = m_size - tmp_header;
            *src2 = m_buffer;
            size2 = header.m_len - (m_size - tmp_header);
        }
        return true;
	}

    //head往前移动sz个单位
    void FetchSize(size_t size)
    {
        if (size > GetOccupySize())
            return;
        m_head = GetIndex(m_head + size);
    }

	int32_t ReadFd(int32_t fd)
	{
		int32_t total_size = 0;
		while(1)
		{
			size_t unused_size = GetFreeSize();
			int32_t read_size = 0;
			if (m_tail < m_head ||  m_head==0)
			{
                read_size = ::read(fd, &m_buffer[m_tail], unused_size);
			}
			else
			{
				struct iovec vec[3];
				vec[0].iov_base = &m_buffer[m_tail];
				vec[0].iov_len = unused_size - (m_head - 1);
				vec[1].iov_base = &m_buffer[0];
				vec[1].iov_len = m_head - 1;
				read_size = ::readv(fd, vec, 2);
			}
			if (read_size <= 0)
			{
				return read_size;
			}
			//update tail
			total_size += read_size;
            m_tail = GetIndex(m_tail + static_cast<uint32_t>(read_size));
            if (IsFull())
                continue;

			break;
		}
		return total_size;
	}

	int32_t WriteFd(int32_t fd)
	{
		int32_t write_size = 0;
		if (IsEmpty()) return 0;
		if (m_head < m_tail)
		{
			write_size = ::write(fd, &m_buffer[m_head], m_tail - m_head);
		}
		else
		{
			struct iovec vec[2];
			vec[0].iov_base = &m_buffer[m_head];
			vec[0].iov_len = Size()+1 - m_head;
			vec[1].iov_base = &m_buffer[0];
			vec[1].iov_len = m_tail;
			write_size = ::writev(fd, vec, 2);
		}
		if (write_size <= 0)
			return write_size;
		m_head = GetIndex(m_head + write_size);
		if (IsEmpty())
		{
			m_head = 0;
			m_tail = 0;
		}
		return write_size;
	}

	size_t GetFreeSize()
	{ 
        return m_tail >= m_head ? (Size() - (m_tail - m_head + 1)) : (m_head - m_tail - 1);
	}

    size_t GetOccupySize()
    {
        return m_size - (GetFreeSize() + 1);
    }

	size_t Size()
	{
		return m_size;
	}

	bool IsEmpty()
	{
		return m_head == m_tail;
	}

	bool IsFull()
	{
		return GetIndex(m_tail + 1) == m_head;
	}

private:
    template<class T>
	bool ReadBuffer(T& t)
	{
        size_t occSize = GetOccupySize();
        if (GetOccupySize() < sizeof(T))
            return false;

        char* dest = (char*)&t;
        if (m_size - m_head >= sizeof(T))
        {
            memcpy(dest, m_buffer + m_head, sizeof(T));
        }
        else
        {
            memcpy(dest, m_buffer + m_head, m_size - m_head);
            memcpy(dest + m_size - m_head, m_buffer, sizeof(T) - (m_size - m_head));
        }
		return true;
	}

	size_t GetIndex(size_t new_index)
	{
        return new_index & (m_size - 1);
	}

    //自动扩容机制
	void DoAllocator(size_t new_size)
	{
		if (Size() >= new_size)
			return;
		new_size = FixSize(new_size);
		char* ptr = new char[new_size];
		if (!IsEmpty())
		{
			if(m_tail > m_head)
			{

				memcpy(ptr, m_buffer + m_head, m_tail - m_head);
                m_tail = m_tail - m_head;
			}
			else
			{
				memcpy(ptr, m_buffer + m_head, m_size - m_head);
				memcpy(ptr + m_size - m_head, m_buffer, m_tail);
                m_tail = m_size - m_head + m_tail;
			}
			m_head = 0;
		}
		else
		{
			m_head = 0;
			m_tail = 0;
		}
		m_size = new_size;
        SAFE_DELETE_ARRAY(m_buffer);
		m_buffer = ptr;
	}

    static size_t FixSize(size_t size) {
        if (IS_POWER_OF_2(size))
            return size;

        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;
        return size + 1;
    }

private:
	char* m_buffer;
	size_t m_size;

	size_t m_head;
	size_t m_tail;
};

#endif