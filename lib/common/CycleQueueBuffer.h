#ifndef CYCLE_QUEUE_BUFFER_H__
#define CYCLE_QUEUE_BUFFER_H__

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/uio.h>
#include <cmath>
#include <vector>

#define M1 1048576 //1024*1204

class CycleQueueBuffer
{
public:
	const static uint32_t INIT_SIZE = 4096;
	CycleQueueBuffer()
		:m_buffer(NULL),m_size(0),m_head(0),m_tail(0)
	{
		DoAllocator(INIT_SIZE);
		m_head = 15;
		m_tail = 15;
	}
	~CycleQueueBuffer()
	{
		if(m_buffer)
		{
			delete[] m_buffer;
			m_buffer = NULL;
		}
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
		if (len > GetUnusedSize())
		{
			DoAllocator(Size() + (len - GetUnusedSize()));
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

	template<class T>
	bool ReadMsg(T& header, std::string& msg)
	{
		if (!ReadBuffer(m_head, (char*)&header, sizeof(T)))
			return false;
		msg.resize(header.m_len, 0);
		if (!ReadBuffer(GetIndex(m_head + sizeof(T)), &msg[0], header.m_len))
			return false;
		m_head = GetIndex(m_head + sizeof(T) + header.m_len);
		if (IsEmpty())
		{
			m_head = 0;
			m_tail = 0;
		}
		return true;
	}

	int32_t ReadFd(int32_t fd)
	{
		const static uint32_t EXTRA_BUFFER_SIZE = 65536;
		char extrabuf[EXTRA_BUFFER_SIZE];

		int32_t total_size = 0;
		while(1)
		{
			uint32_t unused_size = GetUnusedSize();
			int32_t read_size = 0;
			if (m_tail < m_head ||  m_head==0)
			{
				struct iovec vec[2];
				vec[0].iov_base = &m_buffer[m_tail];
				vec[0].iov_len = unused_size;
				vec[1].iov_base = extrabuf;
				vec[1].iov_len = EXTRA_BUFFER_SIZE;
				read_size = ::readv(fd, vec, 2);
			}
			else
			{
				struct iovec vec[3];
				vec[0].iov_base = &m_buffer[m_tail];
				vec[0].iov_len = Size()+1-m_tail;
				vec[1].iov_base = &m_buffer[0];
				vec[1].iov_len = unused_size - (Size()+1-m_tail);
				vec[2].iov_base = extrabuf;
				vec[2].iov_len = EXTRA_BUFFER_SIZE;
				read_size = ::readv(fd, vec, 3);
			}
			if (read_size <= 0)
			{
				return read_size;
			}
			//update tail
			total_size += read_size;
			uint32_t unsigned_read_size = static_cast<uint32_t>(read_size);
			m_tail = (unsigned_read_size<=unused_size?GetIndex(m_tail + unsigned_read_size):GetIndex(m_tail + unused_size));

			if (unsigned_read_size > unused_size)
			{
				Append(extrabuf, unsigned_read_size - unused_size);
				if (unsigned_read_size == unused_size + EXTRA_BUFFER_SIZE)
				{
					//contiune to read
					continue;
				}
			}
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

	size_t GetUnusedSize()
	{
		return Size() - GetUsedSize();
	}

	size_t GetUsedSize()
	{
		return m_head <= m_tail?(m_tail - m_head):(Size()-(m_head - m_tail-1));
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
	bool ReadBuffer(size_t begin, char* dest, size_t len)
	{
		if (m_tail >= begin)
		{
			if (m_tail - begin < len)
				return false;
			memcpy(dest, &m_buffer[begin], len);
		}
		else
		{
			size_t used_size = Size() - (begin - m_tail - 1);
			if (used_size < len)
				return false;
			size_t to_last_size = Size() + 1 - begin;
			if (to_last_size < len)
			{
				memcpy(dest, &m_buffer[begin], to_last_size);
				memcpy(dest + to_last_size, &m_buffer[0],len - to_last_size);
			}
			else
			{
				memcpy(dest, &m_buffer[begin], len);
			}
		}
		return true;
	}

	size_t GetIndex(size_t new_index)
	{
		return new_index % (Size()+1);
	}

	void DoAllocator(size_t new_size)
	{
		if (Size() >= new_size)
			return;
		new_size = CeilToPow2(new_size);
		char* ptr = new char[new_size + 1];
		if (!IsEmpty())
		{
			size_t used_size = GetUsedSize();
			if(m_tail > m_head)
			{
				memcpy(&ptr[0], &m_buffer[m_head], used_size);
			}
			else
			{
				size_t to_last_size = used_size - m_tail;
				memcpy(&ptr[0],&m_buffer[m_head], to_last_size);
				memcpy(&ptr[to_last_size], &m_buffer[0],m_tail);
			}
			m_head = 0;
			m_tail = used_size;
		}
		else
		{
			m_head = 0;
			m_tail = 0;
		}
		m_size = new_size;
		if(m_buffer)
		{
			delete[] m_buffer;
			m_buffer = NULL;
		}
		m_buffer = ptr;
	}

	static size_t CeilToPow2(size_t x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		for (size_t i = 1; i < sizeof(size_t); i <<= 1) {
			x |= x >> (i << 3);
		}
		++x;
		if (x > M1)
		{
			//大于1M时，扩大成能容纳的最大M空间即可
			x = std::ceil((x*1.0) / M1) * M1;
		}

		return x;
	}

private:
	char* m_buffer;
	size_t m_size;

	size_t m_head;
	size_t m_tail;
};

#endif