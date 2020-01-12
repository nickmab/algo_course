#include <algorithm>
#include <exception>
#include <limits>
#include <sstream>

#include "algo_lib/union_find.h"

namespace mabz {

void UnionFind::Union(int a, int b)
{
	auto aIt = mRoots.find(a);
	auto bIt = mRoots.find(b);
	const auto end = mRoots.end();

	if (aIt == end)
	{
		if (bIt == end)
		{
			// neither exists yet; make one the other's root.
			mRoots[a] = b;
			mInverseRoots.insert(std::make_pair(b, a));
			
			if (a != b)
			{
				mRoots[b] = b; // make b its own root here
				mInverseRoots.insert(std::make_pair(b, b));
			}
		}
		else
		{
			// b exists and a does not; make a's root the same as b's.
			const int rootOfB = bIt->second;
			mRoots[a] = rootOfB;
			mInverseRoots.insert(std::make_pair(rootOfB, a));
		}
	}
	else
	{
		if (bIt == end)
		{
			//a exists and b does not; make b's root the same as a's 
			const int rootOfA = aIt->second;
			mRoots[b] = rootOfA;
			mInverseRoots.insert(std::make_pair(rootOfA, b));
		}
		else
		{
			// both exist; make a's root the same as b's (unless they're already the same!)
			const int rootOfA = aIt->second;
			const int rootOfB = bIt->second;

			if (rootOfA	!= rootOfB)
			{
				mRoots[a] = rootOfB;
				mInverseRoots.insert(std::make_pair(rootOfB, a));

				auto range = mInverseRoots.equal_range(rootOfA);
				for (auto currChildOfA = range.first; currChildOfA != range.second; /* do not it++ here */)
				{
					mRoots[currChildOfA->second] = rootOfB;
					mInverseRoots.insert(std::make_pair(rootOfB, currChildOfA->second));
					// deletion has the effect of advancing the iterator.
					currChildOfA = mInverseRoots.erase(currChildOfA);
				}
			}
		}
	}
}

bool UnionFind::Connected(int a, int b) const
{
	auto aIt = mRoots.find(a);
	auto bIt = mRoots.find(b);
	const auto end = mRoots.end();
	
	if (aIt == end)
	{
		std::stringstream err;
		err << "Tried to see whether value \"" << a << "\" was connected to value \"" 
			<< b << "\", but \"" << a << "\" does not even exist in the union!";
		throw std::exception(err.str().c_str());
	}
	else if (bIt == end)
	{
		std::stringstream err;
		err << "Tried to see whether value \"" << a << "\" was connected to value \"" 
			<< b << "\", but \"" << a << "\" does not even exist in the union!";
		throw std::exception(err.str().c_str());
	}
	else
	{
		return aIt->second == bIt->second;
	}
}

int UnionFind::Find(int a) const
{
	auto aIt = mRoots.find(a);
	const auto end = mRoots.end();
	
	if (aIt == end)
	{
		std::stringstream err;
		err << "Tried to find value \"" << a << "\" in the union, but it's not there!";
		throw std::exception(err.str().c_str());
	}
	else
	{
		int maxFound = std::numeric_limits<int>::min();
		auto range = mInverseRoots.equal_range(aIt->second);
		for (auto aRelative = range.first; aRelative != range.second; ++aRelative)
		{
			maxFound = std::max(maxFound, aRelative->second);
		}

		return maxFound;
	}
}

} /* namespace mabz */