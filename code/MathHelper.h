/************************************************************
* Name: Patrick Guo
* ID: 11378369
************************************************************/
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include "GeneralHelper.h"
#include <math.h>

class MathHelper
{
public:
	/************************************************************
	* This function takes the values from two
	* maps and treats them as vectors and then dot products
	* them. The only values counted are the ones retrieved
	* with the same key for both maps
	************************************************************/
	static double GetDotProduct(std::map<int, double> &lhs, std::map<int, double> &rhs)
	{
		double dot_product = 0.0;

		for (std::map<int, double>::iterator i = lhs.begin(); i != lhs.end(); i++)
		{
			int current_lhs_key = i->first;
			double current_lhs_value = i->second;
			if (GeneralHelper::MapContainsKey(rhs, current_lhs_key))
			{
				double current_rhs_value = rhs[current_lhs_key];
				dot_product += (current_lhs_value * current_rhs_value);
			}
		}

		return dot_product; //magnitudes
	}

	static double GetMagnitude(std::map<int, double> &values)
	{
		double sum_of_squared_values = 0.0;

		for (std::map<int, double>::iterator i = values.begin(); i != values.end(); i++)
		{
			double current_value = i->second;
			sum_of_squared_values += pow(current_value, 2.0);
		}

		return sqrt(sum_of_squared_values);
	}
};

#endif