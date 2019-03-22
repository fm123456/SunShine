#ifndef RNADOM_H_
#define RNADOM_H_

#include <ctime>
#include <algorithm>
#include <stdint.h>

#include "Singleton.h"

class Random : public Singleton<Random>
{
public:
	Random()
	{
		std::srand((unsigned int)std::time(nullptr));
	}
	~Random()
	{
	}

	/*返回一个x到y之间的一个随机数（包括x但不包括y）
	*/
	int32_t RandInteger(int32_t x, int32_t y)
	{
		int32_t min_val = std::min(x, y);
		int32_t max_val = std::max(x, y);
		if (min_val == max_val)
			return max_val;

		return (std::rand() % (max_val - min_val)) + min_val;
	}
};

#endif
