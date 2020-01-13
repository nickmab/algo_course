#pragma once

#include <exception>
#include <functional>
#include <sstream>
#include <string>

#include <algo_lib/exceptions.h>

namespace mabz { namespace search {

// Look for value "target" in already-sorted container "cont" specifically between 
// container indices [begin, end] inclusive. Can optionally provide an equality 
// comparison function (e.g. for comparing doubles), otherwise defaults 
// to using == operator. Return -1 if not found.
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
			return halfWay;
		}
		else if (cmpFunc(halfwayVal,  target))
		{
			if (eqFunc(cont[begin], target)) return begin;
			if (halfWay == begin) break;
			++begin;
			end = halfWay;
		}
		else
		{
			if (eqFunc(cont[end], target)) return end;
			--end;
			begin = halfWay;
		}
	}

	return eqFunc(cont[begin], target) ? begin : -1;
}

// Wrapper for use with reverse-sorted containers if you don't want to bother 
// personally specifying a non-default equality comparison function.
template <typename T, typename Container>
int ReverseBinSearch(const T& target, const Container& cont, int begin, int end,
	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
{
	return BinSearch<T,Container>(target, cont, begin, end, eqFunc, true);
}

} /* namespace search */
} /* namespace mabz */
