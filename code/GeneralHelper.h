/************************************************************
* Name: Patrick Guo
* ID: 11378369
************************************************************/
#ifndef GENERAL_HELPER_H
#define GENERAL_HELPER_H

#include <map>

struct EstimatedUserRatings;

class GeneralHelper
{
public:
	static bool MapContainsKey(std::map<int, double> &my_map, int key)
	{
		std::map<int, double>::iterator it;
		it = my_map.find(key);
		if (it != my_map.end())
		{
			return true;
		}

		return false;
	}

	struct ArrayOf5
	{
		int Values[5];

		ArrayOf5()
		{

		}

		ArrayOf5(int *values)
		{
			for (int i = 0; i < 5; i++)
			{
				Values[i] = values[i];
			}
		}
	};
};

#endif
