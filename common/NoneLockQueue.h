#ifndef NONE_LOCK_QUEUE_H__
#define NONE_LOCK_QUEUE_H__

/****************************************************************************

无锁消息队列，目前只适用于两个线程间的数据传输。即一个生产者一个消费者的模式
*****************************************************************************/


#include <atomic>
#include <thread>
#include <list>
#include <mutex>

template <class T>
class NoneLockQueue
{
private:
	struct QueueNode
	{
		QueueNode()
			:m_next(NULL)
		{
		}
		QueueNode(const T& data)
			:m_data(data), m_next(NULL)
		{
		}

		T m_data;
		QueueNode* m_next;
	};

public:
	NoneLockQueue()
	{
		QueueNode* header = new QueueNode();
		m_head.store(header, std::memory_order_relaxed);
		m_tail.store(header, std::memory_order_relaxed);
		m_size = 0;
	}

	~NoneLockQueue()
	{
	}

	void PushBack(const T& data)
	{
		QueueNode* node = new QueueNode(data);
		m_tail.load(std::memory_order_acquire)->m_next = node;
		m_tail.store(node, std::memory_order_release);
		m_size.fetch_add(1, std::memory_order_release);
	}

	bool PopFront(T& data)
	{
		if (Empty())
			return false;
		QueueNode* cur = m_head;
		m_head.store(cur->m_next, std::memory_order_release);
		m_size.fetch_sub(1, std::memory_order_release);

		data = m_head.load(std::memory_order_acquire)->m_data;
		delete cur;
		cur = NULL;

		return true;
	}

	bool Empty()
	{
		return m_head.load(std::memory_order_acquire) == m_tail.load(std::memory_order_acquire);
	}

	size_t Size()
	{
		return m_size.load(std::memory_order_acquire);
	}
private:
	std::atomic<QueueNode*> m_head;
	std::atomic<QueueNode*> m_tail;
	std::atomic<size_t> m_size;
};

#endif