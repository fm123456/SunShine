#ifndef _KB_SINGLETON_H_
#define _KB_SINGLETON_H_

#include <stdlib.h>

#ifdef WIN32
#include <mutex>
#else
#include <pthread.h>
#endif

template<class T>
class Singleton
{
public:
    static T& instance()
    {
#ifdef WIN32
		if (!m_instance)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			if (!m_instance)
			{
				T* tmp = new T();
				m_instance = tmp;
			}
		}
#else
		pthread_once(&m_ponce, &Singleton::init);
#endif
		return *m_instance;
    }

private:
#ifdef WIN32
	static std::mutex m_mutex;
#else
    static void init()
    {
        m_instance = new T();
        atexit(destroy);
    }
    static void destroy()
    {
        if(m_instance)
        {
                delete m_instance;
                m_instance = 0;
        }
    }
	static pthread_once_t m_ponce;
#endif

private:
    static T* volatile m_instance;
    
};

template<class T>
T* volatile Singleton<T>::m_instance = NULL;

#ifdef WIN32
template<class T>
std::mutex Singleton<T>::m_mutex;
#else
template<typename T>
pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;
#endif

#endif

