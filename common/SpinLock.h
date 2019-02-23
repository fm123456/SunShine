#ifndef SPIN_LOCK_H_
#define SPIN_LOCK_H_

#include <atomic>

class SpinMutex
{
public:
	SpinMutex() : m_flag(false)
	{
	}

	void lock()
	{
		bool expected = false;
		while (!m_flag.compare_exchange_weak(expected, true))
		{
			expected = false;
		}
	}

	void unlock()
	{
		m_flag.store(false);
	}

private:
	std::atomic<bool> m_flag;
};

class SpinLock
{
public:
	SpinLock(SpinMutex& mutex) : m_mutex(mutex)
	{
		m_mutex.lock();
	}

	~SpinLock()
	{
		m_mutex.unlock();
	}

private:
	SpinMutex& m_mutex;
};

#endif
