#pragma once

#include <exception>
#include <functional>
#include <sstream>
#include <string>

#include <algo_lib/exceptions.h>

namespace mabz { namespace search {

// Look for value "target" in already-sorted container "cont" specifically between 
// container indices [begin, end] inclusive. Can optionally provide an equality 
// comparison function, otherwise defaults to using == operator. 
// Relies on the type supporting < and > comparison operators.
// Return -1 if not found.
// Returns the left-most index (if there are multiple instances of the same value).
template <typename T, typename Container>
int BinSearch(const T& target, const Container& cont, int begin, int end,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; },
	bool reverseSorted=false)
{
	if (end < begin || begin < 0 || end >= cont.size())
	{
		std::stringstream err;
		err << "Attempted to call BinSearch with illegal arguments. " << std::endl
		    << "end must be >= begin, begin and end must be within the "
		    << "valid range 0-" << cont.size()-1 << " for the provided container. " << std::endl
		    << "Got begin(" << begin << ") and end(" << end << ").";
		throw mabz::IndexOutOfRange(err.str());
	}

	// when we find a matching desired value, use this func to return the left-most
	// instance of it. 
	auto returnLeftmost = [&] (int idx) ->int {
		while (idx > 0 && eqFunc(cont[idx], cont[idx-1])) --idx;
		return idx;
	};

	std::function<bool(T,T)> cmpFunc;
	if (reverseSorted)
	{
		cmpFunc = [] (T a, T b) ->bool { return a < b; };
	}
	else
	{
		cmpFunc = [] (T a, T b) ->bool { return a > b; };
	}

	while (begin != end)
	{
		int halfWay = (begin + end) / 2;
		const T halfwayVal = cont[halfWay];
		if (eqFunc(halfwayVal, target))
		{
			return returnLeftmost(halfWay);
		}
		else if (cmpFunc(halfwayVal,  target))
		{
			if (eqFunc(cont[begin], target)) return returnLeftmost(begin);
			if (halfWay == begin) return -1; // we've searched everything and failed.
			++begin;
			end = halfWay;
		}
		else
		{
			if (eqFunc(cont[end], target)) return returnLeftmost(end);
			--end;
			begin = halfWay;
		}
	}

	return eqFunc(cont[begin], target) ? returnLeftmost(begin) : -1;
}

// Wrapper for use with reverse-sorted containers if you don't want to bother 
// personally specifying a non-default equality comparison function.
template <typename T, typename Container>
int ReverseBinSearch(const T& target, const Container& cont, int begin, int end,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
{
	return BinSearch(target, cont, begin, end, eqFunc, true);
}

} /* namespace search */
} /* namespace mabz */
