#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include <algo_lib/binary_search.h>

namespace mabz { namespace search {

template <typename T>
class ThreeSum
{
	template <typename T>
	class Triplet
	{
		T first;
		T second;
		T third;
	};

private:
	T mTargetSum;
	// for use in binary search and checking whether the triplet sum is equal to the desired val.
	// defaults to using the == operator if unspecified.
	bool (*mEqualityFunction)(T, T){nullptr};
	std::vector<T> mSortedInputs;
	std::vector<Triplet<T> > mResults;
	bool mDone;

public:
	ThreeSum(T targetSum, const std::vector<T>& inputs, bool(*eqFunc)(T, T)=nullptr);
	
	void Run();
	bool IsDone() const { return mDone; }
	int GetNumberOfTriplets() const 
	{ 
		if (!mDone) throw "Tried to call GetNumberOfTriplets before calling Run!";
		return mResults.size(); 
	}
	
	std::vector<Triplet<T> > GetResults() const
	{
		if (!mDone) throw "Tried to call GetResults before calling Run!";
		return mResults;
	}
};

template <typename T>
ThreeSum<T>::ThreeSum(T targetSum, const std::vector<T>& inputs, bool(*eqFunc)(T, T))
	: mTargetSum(targetSum)
	, mEqualityFunction(eqFunc)
	, mSortedInputs(inputs)
	, mDone(false)
{
	std::sort(mSortedInputs.begin(), mSortedInputs.end());
}

template <typename T>
void ThreeSum<T>::Run()
{
	if (mEqualityFunction)
	{
		std::cout << BinSearch(mSortedInputs[0], mSortedInputs, 0, mSortedInputs.size()-1, mEqualityFunction) << std::endl;
		std::cout << BinSearch(mSortedInputs[1], mSortedInputs, 0, mSortedInputs.size()-1, mEqualityFunction) << std::endl;
		std::cout << BinSearch(mSortedInputs[1], mSortedInputs, 2, mSortedInputs.size()-1, mEqualityFunction) << std::endl;
		std::cout << BinSearch(mSortedInputs[mSortedInputs.size()-1], mSortedInputs, 2, mSortedInputs.size()-1, mEqualityFunction) << std::endl;
		std::cout << BinSearch(mSortedInputs[0] + 99999, mSortedInputs, 0, mSortedInputs.size()-1, mEqualityFunction) << std::endl;
		std::cout << BinSearch(mSortedInputs[0] + 99999, mSortedInputs, 0, mSortedInputs.size()-1, mEqualityFunction) << std::endl;
	}
	else
	{
		std::cout << BinSearch(mSortedInputs[0], mSortedInputs, 0, mSortedInputs.size()-1) << std::endl;
		std::cout << BinSearch(mSortedInputs[1], mSortedInputs, 0, mSortedInputs.size()-1) << std::endl;
		std::cout << BinSearch(mSortedInputs[1], mSortedInputs, 2, mSortedInputs.size()-1) << std::endl;
		std::cout << BinSearch(mSortedInputs[mSortedInputs.size()-1], mSortedInputs, 2, mSortedInputs.size()-1) << std::endl;
		std::cout << BinSearch(mSortedInputs[0] + 99999, mSortedInputs, 0, mSortedInputs.size()-1) << std::endl;
		std::cout << BinSearch(mSortedInputs[0] + 99999, mSortedInputs, 0, mSortedInputs.size()-1) << std::endl;	
	}

	mDone = true;
}

} /* namespace search */
} /* namespace mabz */