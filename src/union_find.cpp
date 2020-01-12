#include <algorithm>
#include <exception>
#include <limits>
#include <set>
#include <sstream>
#include <vector>

#include "algo_lib/union_find.h"

namespace mabz {

void UnionFind::CheckArrayBounds(int i) const
{
	if (i < 0 || i >= mCapacity)
	{
		std::stringstream err;
		err << "Index out of bounds! Must be [0, " << mCapacity << "). "
			<< "Instead got " << i << ".";
		throw std::exception(err.str().c_str());
	}
}

int UnionFind::GetRoot(int i) const
{
	int ultimateRoot{mRoots[i]};
	while (ultimateRoot != mRoots[ultimateRoot])
	{
		ultimateRoot = mRoots[ultimateRoot];
	}
	return ultimateRoot;
}

void UnionFind::Union(int a, int b)
{
	CheckArrayBounds(a);
	CheckArrayBounds(b);

	const int rootOfA{GetRoot(a)};
	const int rootOfB{GetRoot(b)};

	if (rootOfA == rootOfB) return;

	// link the root of the smaller tree to the root of the larger tree
	if (mTreeSizes[rootOfA] > mTreeSizes[rootOfB])
	{
		mRoots[rootOfB] = rootOfA;
		mTreeSizes[rootOfA] += mTreeSizes[rootOfB];
	}
	else
	{
		mRoots[rootOfA] = rootOfB;
		mTreeSizes[rootOfB] += mTreeSizes[rootOfA];
	}
}

bool UnionFind::Connected(int a, int b) const
{
	CheckArrayBounds(a);
	CheckArrayBounds(b);

	const int rootOfA{GetRoot(a)};
	const int rootOfB{GetRoot(b)};
	
	return rootOfA == rootOfB;
}

int UnionFind::Find(int a) const
{
	CheckArrayBounds(a);

	// not implemented
	return 0;
}

} /* namespace mabz */