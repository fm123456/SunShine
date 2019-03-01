#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include <stdint.h>
#include <set>
#include <vector>
#include <map>
#include "Singleton.h"

#define INVALID_INDEX -1
#define DEFAULT_ARRAY_SIZE 100

template <class T>
class ObjectPool : public Singleton<ObjectPool<T>>
{
private:
	typedef T* object_data_p_t;
	typedef std::vector<object_data_p_t> object_data_p_vec_t;
	typedef std::map<object_data_p_t, int32_t> object_data_p_map_t; //data <---> num_index
	typedef std::map<int32_t, std::set<int32_t>> object_unused_map_t;  // num_index <----> unused_set

public:
	~ObjectPool()
	{
		for (auto it : m_data_vec)
		{
			delete[] it;
		}
		m_data_vec.clear();
		m_data_map.clear();
		m_unused_map.clear();
	}

	/* 这里需要做一个说明，因为是对象池的概念。对象是重复使用的。被对象池管理的对象需要定一个reset方法来重置本对象的内容
	*/
	T* GetNewObject()
	{
		//拿空闲的对象
		object_data_p_t ins = NULL;

		auto it = m_unused_map.begin();
		for (; it != m_unused_map.end(); )
		{
			if (it->second.empty())
			{
				//理论上这个这个逻辑是进不来的，这次防止出错
				it = m_unused_map.erase(it);
				continue;
			}
			int32_t num_index = it->first;
			int32_t innew_index = (*it->second.begin());
			it->second.erase(it->second.begin());
			if (it->second.empty())
				m_unused_map.erase(it);

			ins = ((object_data_p_t)((char*)(m_data_vec[num_index]) + sizeof(T)*innew_index));
			break;
		}
		//new object array
		if (!ins)
		{
			ins = new T[DEFAULT_ARRAY_SIZE];
			m_data_vec.push_back(ins);

			int32_t new_array_index = m_data_vec.size() - 1;
			m_data_map[ins] = new_array_index;
			for (int32_t i = 1; i < DEFAULT_ARRAY_SIZE; i++)
				m_unused_map[new_array_index].insert(i);
		}
		ins->reset();
		return ins;
	}

	void DestroyObject(T* t)
	{
		if (!t || m_data_map.empty()) 
			return;

		auto it = m_data_map.lower_bound(t);
		if (it == m_data_map.begin() && it->first != t)
			return;
		if (it == m_data_map.end() || it->first != t)
			it--;

		int32_t dis = (int32_t)t - (int32_t)it->first;
		int32_t temp_index = dis / sizeof(T);
		if (temp_index >= DEFAULT_ARRAY_SIZE)
			return;

		m_unused_map[it->second].insert(temp_index);
	}

private:
	object_data_p_map_t m_data_map;
	object_data_p_vec_t m_data_vec;

	object_unused_map_t m_unused_map;
};

#endif
