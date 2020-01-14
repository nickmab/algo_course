#pragma once

#include <functional>
#include <iostream>
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
// Returns the left-most index (if there are multiple equal highs).
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

	// by default we give back the left-most index (if there are multiple equal highs).
	while (begin > 0 && eqFunc(cont[begin], cont[begin-1])) --begin;
	return begin;
}

// Look for value "target" in bitonic container "cont".
// "bitonic" means that consecutive values from left to right are monotonic
// increasing up to a point and then monotonic decreasing from that point.
// Can optionally provide an equality comparison function 
// (e.g. for comparing doubles), otherwise defaults to 
// using == operator. Return -1 if not found.
// Also works if the entire container is monotonic increasing or decreasing.
// Returns the left-most index (if there are multiple instances of the same value).
template <typename T, typename Container>
int BitonicSearch(const T& target, const Container& cont,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
{
	const int turningPoint = FindBitonicTurningPoint<T>(cont, eqFunc);
	const int uBound = cont.size() - 1;
	int result{-1};
	
	if (turningPoint > 0)
	{
		result = BinSearch(target, cont, 0, turningPoint, eqFunc);

		// only need to keep searching if we didn't already find it!
		if (result == -1 && turningPoint < uBound)
		{
			result = ReverseBinSearch(target, cont, turningPoint + 1, uBound, eqFunc);
		}
	}
	else
	{
		result = ReverseBinSearch(target, cont, 0, uBound, eqFunc);
	}
	
	// Returns the left-most index (if there are multiple instances of the same value).
	while (result > 0 && eqFunc(cont[result], cont[result-1])) --result;
	return result;
}

// Same as Bitonic search, but without first looking for the turning point.
// Returns the left-most index (if there are multiple instances of the same value).
template <typename T, typename Container>
int BetterBitonicSearch(const T& target, const Container& cont,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
{
	int begin = 0;
	int end = cont.size()-1;	

	// when we find a matching desired value, use this func to return the left-most
	// instance of it. 
	auto returnLeftmost = [&] (int idx) ->int {
		while (idx > 0 && eqFunc(cont[idx], cont[idx-1])) --idx;
		return idx;
	};

	for (;;)
	{
		int halfWay = (begin + end) / 2;
		if (halfWay == begin)
		{
			// we are down to the last two elements.
			if (eqFunc(target, cont[begin])) return begin;
			if (eqFunc(target, cont[end])) return end;
			return -1;
		}
		
		const T halfWayVal = cont[halfWay];
		if (eqFunc(target, halfWayVal)) return returnLeftmost(halfWay);

		// peek left and right to determine which way to go next.
		int iLeft = halfWay - 1;
		int iRight = halfWay + 1;
		for ( ; iLeft > begin && eqFunc(cont[iLeft], halfWayVal); --iLeft) {}
		for ( ; iRight < end && eqFunc(cont[iRight], halfWayVal); ++iRight) {}
		const T valLeft = cont[iLeft];
		const T valRight = cont[iRight];

		// we need to check the equality functions first, otherwise it might
		// mess up a double comparison (for example) and start going the wrong way.
		if (eqFunc(target, valLeft)) return returnLeftmost(iLeft);
		if (eqFunc(target, valRight)) 
		{
			// stay faithful to our policy of returning left-most...
			if (target < valLeft)
			{
				const int leftSearch = BinSearch(target, cont, begin, iLeft, eqFunc);
				if (leftSearch != -1) return leftSearch;
			}
			return returnLeftmost(iRight);
		}
		
		if (target > halfWayVal)
		{
			if (valLeft > halfWayVal)
			{
				if (valRight > halfWayVal)
				{
					// we've found a lowpoint midpoint; this should be impossible.
					std::stringstream err;
					err << "Found higher values to both the right and right of index " << halfWay
					    << "Value at idx: " << halfWayVal << " and vals to left and right: "
					    << valLeft << ", " << valRight << ".";
					throw mabz::IllegalArgumentException(err.str());
				}
				else
				{
					// just keep going left.
					end = iLeft;
				}
			}
			else
			{
				if (valRight > halfWayVal)
				{
					// just keep going right.
					begin = iRight;
				}
				else
				{
					// our target is higher than the half way val we just checked,
					// but the half way val happens to be the highest in the whole container.
					return -1; 
				}
			}
		}
		else
		{
			if (valLeft < halfWayVal)
			{
				if (valRight < halfWayVal)
				{
					// we've found a maximum midpoint; simply binsearch both directions!
					// note that the binary search functions automatically return the left-most.
					const int leftSearch = BinSearch(target, cont, begin, iLeft, eqFunc);
					if (leftSearch != -1) return leftSearch;
					return ReverseBinSearch(target, cont, iRight, end, eqFunc);
				}
				else
				{
					// just keep going left.
					end = iLeft;
				}
			}
			else
			{
				if (valRight < halfWayVal)
				{
					// just keep going right.
					begin = iRight;
				}
				else
				{
					// our target is lower than the half way val we just checked,
					// but the half way val happens to be the lowest in the whole container.
					// This should be impossible!
					std::stringstream err;
					err << "Found higher values to both the right and right of index " << halfWay
					    << "Value at idx: " << halfWayVal << " and vals to left and right: "
					    << valLeft << ", " << valRight << ".";
					throw mabz::IllegalArgumentException(err.str());
				}
			}
		}
	}

	throw mabz::Unreachable("BetterBitonicSearch somehow escaped from an infinite loop.");
	// just to stop the compiler from complaining that not all code paths return a value.
	return -1;
}

} /* namespace search */
} /* namespace mabz */
