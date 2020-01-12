#pragma once

#include <map>

namespace mabz {

class UnionFind
{
private:
	// the number at key "i" is the index/number of the number i's root. 
	std::map<int,int> mRoots;

	// the number at key "i" is the root and the values are numbers that connect to that root.
	// thus can have multiple keys of the same value.
	std::multimap<int,int> mInverseRoots;

public:
	void Union(int, int); // can add a value with no connectsion by passing same val to both args.
	bool Connected(int, int) const;
	int Find(int) const;

	void Reset() { mRoots = std::map<int,int>(); mInverseRoots = std::multimap<int,int>(); }
};

} /* namespace mabz */