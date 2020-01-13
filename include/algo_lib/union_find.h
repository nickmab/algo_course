#pragma once

namespace mabz {

class UnionFind
{
private:
	int mCapacity;
	// Integer array. The number at position "i" is the index/number of the number i's root. 
	int* mRoots{nullptr};
	// keep track of the number of nodes in the tree rooted at index "i".
	int* mTreeSizes{nullptr};

	void CheckArrayBounds(int) const;
	int GetRoot(int) const;

public:
	UnionFind(int capacity) 
		: mCapacity(capacity)
		, mRoots(new int[capacity]())
		, mTreeSizes(new int[capacity]())
	{ 
		Reset(); 
	}

	void Reset() 
	{ 
		for (int i = 0; i < mCapacity; i++) 
		{
			mRoots[i] = i;
			mTreeSizes[i] = 1;
		}
	}
	~UnionFind() 
	{ 
		delete[] mRoots; 
		delete[] mTreeSizes; 
	} 
	
	UnionFind() = delete;
	UnionFind(const UnionFind&) = delete;
	UnionFind(UnionFind&&) = delete;

	void Union(int, int);
	bool Connected(int, int) const;
};

} /* namespace mabz */