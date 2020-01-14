#pragma once

#include <functional>
#include <sstream>
#include <string>

#include <algo_lib/binary_search.h>
#include <algo_lib/exceptions.h>

namespace mabz { namespace search {

// Look for index number of the turning point / peak in bitonic container "cont".
// "bitonic" means that consecutive values from left to right are monotonic
// increasing up to a point and then monotonic decreasing from that point.
// Can optionally provide an equality comparison function 
// (e.g. for comparing doubles), otherwise defaults to 
// using == operator.
template <typename T, typename Container>
int FindBitonicTurningPoint(const Container& cont,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
{
	// find the index of the turning point
	int begin = 0;
	int end = cont.size()-1;	

	while (begin != end)
	{
		const T beginVal = cont[begin];
		const T endVal = cont[end];
		
		int halfWay = (begin + end) / 2;
		if (halfWay == begin)
		{
			// we are down to the last two elements.
			if (!eqFunc(endVal, beginVal) && endVal > beginVal)
			{
				begin = end;
			}
			break;
		}
		
		const T halfWayVal = cont[halfWay];

		// compare the value in the middle of our search range with those at the 
		// ends of the range. Looking from left to right, these values could be any 
		// combination of highest, lowest or equal, in any order. Hence all these comparisons.
		if (eqFunc(halfWayVal, beginVal))
		{
			if (eqFunc(halfWayVal, endVal))
			{
				// literally the beginning, middle and end values are equal!
				goto AmbiguousDirection;
			}
			else if (halfWayVal > endVal)
			{
				// "half way" must be at or to the right of the peak. 
				end = halfWay;
			}
			else
			{
				begin = halfWay;
			}
		}
		else // ...and so on...
		{
			if (eqFunc(halfWayVal, endVal))
			{
				if (halfWayVal < beginVal)
				{
					end = halfWay;
				}
				else
				{
					begin = halfWay;
				}
			}
			else
			{
				// this indicates the middle value is higher than both ends.
				goto AmbiguousDirection;
			}
		}

		continue;
AmbiguousDirection:
		// in this case we'll peek left and right to pick a direction.
		// first we look left...
		int i = halfWay;
		for ( ; i > begin && eqFunc(cont[i], halfWayVal); --i) {}
		if (cont[i] > halfWayVal)
		{
			end = i + 1;
		}
		else
		{
			// else we look right...
			i = halfWay;
			for ( ; i < end && eqFunc(cont[i], halfWayVal); ++i) {}
			if (cont[i] > halfWayVal)
			{
				begin = i - 1;
			}
			else
			{
				// this means that when we go either left or right,
				// we only encountered equal or smaller values. 
				return halfWay;
			}
		}
	}

	return begin;
}

// Look for value "target" in bitonic container "cont".
// "bitonic" means that consecutive values from left to right are monotonic
// increasing up to a point and then monotonic decreasing from that point.
// Can optionally provide an equality comparison function 
// (e.g. for comparing doubles), otherwise defaults to 
// using == operator. Return -1 if not found.
// Also works if the entire container is monotonic increasing or decreasing.
template <typename T, typename Container>
int BitonicSearch(const T& target, const Container& cont,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
{
	const int turningPoint = FindBitonicTurningPoint<T>(cont, eqFunc);
	const int uBound = cont.size() - 1;
	int result{-1};
	
	if (turningPoint > 0)
	{
		result = BinSearch(target, cont, 0, turningPoint);

		// only need to keep searching if we didn't already find it!
		if (result == -1 && turningPoint < uBound)
		{
			result = ReverseBinSearch(target, cont, turningPoint + 1, uBound);
		}
	}
	else
	{
		result = ReverseBinSearch(target, cont, 0, uBound);
	}
	
	return result;
}

} /* namespace search */
} /* namespace mabz */
