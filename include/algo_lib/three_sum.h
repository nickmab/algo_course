#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

#include <algo_lib/binary_search.h>
#include <algo_lib/exceptions.h>

namespace mabz { namespace search {

template <typename T, typename Container>
class ThreeSum
{
public:
	template <typename T>
	struct Triplet
	{
		T first;
		T second;
		T third;
		Triplet(T a, T b, T c) : first(a), second(b), third(c) {}
	};

private:
	T mTargetSum;
	// for use in binary search and checking whether the triplet sum is equal to the desired val.
	// defaults to using the == operator if unspecified.
	typedef std::function<bool(T,T)> CMPFunc;
	CMPFunc mEqualityFunction;
	Container mSortedInputs;
	std::vector<Triplet<T> > mResults;
	bool mDone;

public:
	ThreeSum(T targetSum, const Container& inputs, CMPFunc eqFunc=CMPFunc{nullptr});
	
	void Run();
	bool IsDone() const { return mDone; }
	int GetNumberOfTriplets() const 
	{ 
		if (!mDone) throw mabz::UnexpectedMethodCall("Tried to call GetNumberOfTriplets before calling Run!");
		return mResults.size(); 
	}
	
	std::vector<Triplet<T> > GetResults() const
	{
		if (!mDone) throw mabz::UnexpectedMethodCall("Tried to call GetResults before calling Run!");
		return mResults;
	}
};

template <typename T, typename Container>
ThreeSum<T,Container>::ThreeSum(T targetSum, const Container& inputs, CMPFunc eqFunc)
	: mTargetSum(targetSum)
	, mEqualityFunction(eqFunc)
	, mSortedInputs(inputs)
	, mDone(false)
{
	std::sort(mSortedInputs.begin(), mSortedInputs.end());
}

template <typename T, typename Container>
void ThreeSum<T,Container>::Run()
{
	if (mDone) throw mabz::UnexpectedMethodCall("Tried to call Run more than once!");

	const int n = mSortedInputs.size();

	std::function<int(T,int)> binSearchCallback;
	if (mEqualityFunction)
	{
		binSearchCallback = [&](T lookFor, int startIdx) ->int { 
			return BinSearch(lookFor, mSortedInputs, startIdx, n-1, mEqualityFunction); 
		};
	}
	else
	{
		binSearchCallback = [&](T lookFor, int startIdx) ->int { 
			return BinSearch(lookFor, mSortedInputs, startIdx, n-1); 
		};
	}

	for (int i = 0; i < (n-2); i++)
	{
		for (int j = i+1; j < (n-1); j++)
		{
			const T thirdValueRequired = mTargetSum - mSortedInputs[i] - mSortedInputs[j];
			const int resultIdxOrMinusOne = binSearchCallback(thirdValueRequired, j+1);
			if (resultIdxOrMinusOne != -1)
			{
				mResults.emplace_back(mSortedInputs[i], mSortedInputs[j], mSortedInputs[resultIdxOrMinusOne]);
			}
		}
	}

	mDone = true;
}

} /* namespace search */
} /* namespace mabz */