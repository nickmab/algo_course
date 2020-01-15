#pragma once

#include <cstdlib>
#include <cstring>
#include <utility>

#include <algo_lib/exceptions.h>

namespace mabz { namespace containers {

template <typename T>
class Deque 
{
private:
	template <typename T>
	struct Node
	{
		T mItem;
		Node<T>* mPrev{nullptr};
		Node<T>* mNext{nullptr};
	};

	Node<T>* mNodePool{nullptr};
	Node<T>* mFirstItem{nullptr};
	Node<T>* mLastItem{nullptr};
	std::size_t mCapacity{1};
	std::size_t mElemCount{0};

	void resize(std::size_t newCapacity);

public:
    Deque() : mNodePool(new Node<T>[1]()) { memset(mNodePool, 0, sizeof(Node<T>)); }
    ~Deque() { delete[] mNodePool; }
    Deque(const Deque&) = delete;
    Deque(Deque&&) = delete;

    bool empty() const { return mElemCount == 0; }
    int size() const { return mElemCount; }
    
    // Adds item to front or back respectively.
    void push_front(T);
    void push_back(T);
    
    // Remove and return the item at the front/back.
    // Throws mabz::EmptyContainer exception if already empty.
    T pop_front();
    T pop_back();

    class iterator;
    iterator begin();
    iterator end();
};

template <typename T>
void Deque<T>::resize(std::size_t newCapacity)
{
	auto newPtr = new Node<T>[newCapacity]();
	memset(newPtr, 0, newCapacity * sizeof(Node<T>));

	Node<T>* oldNode = mFirstItem;
	Node<T>* newNode = newPtr;
	do {
		newNode->mItem = oldNode->mItem;
		if (newNode != newPtr)
		{
			newNode->mPrev = newNode - 1;
			newNode->mPrev->mNext = newNode;
		}
		newNode++;
		oldNode = oldNode->mNext;
	} while (oldNode != nullptr);

	delete[] mNodePool;
	mNodePool = newPtr;
	mFirstItem = mNodePool;
	mLastItem = --newNode;
	mCapacity = newCapacity;
}

template <typename T>
void Deque<T>::push_front(T item)
{
	if (mElemCount == mCapacity) resize(2*mCapacity);

	Node<T>* node = mNodePool + mElemCount;
	node->mItem = item;
	if (mElemCount == 0)
	{
		mLastItem = node;
	}
	else
	{
		mFirstItem->mPrev = node;
		node->mNext = mFirstItem;
	}
	mFirstItem = node;
	mElemCount++;
}

template <typename T>
void Deque<T>::push_back(T item)
{
	if (mElemCount == mCapacity) resize(2*mCapacity);
	
	Node<T>* node = mNodePool + mElemCount;
	node->mItem = item;
	if (mElemCount == 0)
	{
		mFirstItem = node;
	}
	else
	{
		mLastItem->mNext = node;
		node->mPrev	= mLastItem;
	}
	mLastItem = node;
	mElemCount++;
}

template <typename T>
T Deque<T>::pop_front()
{
	if (mElemCount == 0)
	{
		throw mabz::EmptyContainer("Tried to pop_front from empty Deque.");
	}

	T returnValue = mFirstItem->mItem;
	if (mFirstItem != mLastItem)
	{
		mFirstItem = mFirstItem->mNext;
		mFirstItem->mPrev = nullptr;
	}
	mElemCount--;

	if (mCapacity > 4 && mElemCount <= mCapacity/4) resize(mCapacity/2);

	return returnValue;
}

template <typename T>
T Deque<T>::pop_back()
{
	if (mElemCount == 0)
	{
		throw mabz::EmptyContainer("Tried to pop_back from empty Deque.");
	}

	T returnValue = mLastItem->mItem;
	if (mFirstItem != mLastItem)
	{
		mLastItem = mLastItem->mPrev;
		mLastItem->mNext = nullptr;
	}
	mElemCount--;

	if (mCapacity > 4 && mElemCount <= mCapacity/4) resize(mCapacity/2);

	return returnValue;
}

template <typename T>
class Deque<T>::iterator
{

friend class Deque<T>;

private:
	typename Deque<T>::Node<T>* mNode{nullptr};
	bool mAtEnd;

	void ThrowIfNull() const { if (mNode == nullptr) 
		throw mabz::IllegalIteratorOp("Attempted to work with null Deque iterator!"); }

protected:
	iterator(typename Deque<T>::Node<T>* node, bool atEnd=false) 
		: mNode(node)
		, mAtEnd(atEnd)
	{}

public:
	iterator() {}
	iterator(iterator&& other) 
		: mNode(other.mNode) 
		, mAtEnd(other.mAtEnd)
	{}

	iterator& operator = (const iterator& other)
	{
		mNode = other.mNode;
		mAtEnd = other.mAtEnd;
		return *this;
	}

	iterator& operator ++ ()
	{ 
		ThrowIfNull(); 
		if (mNode->mNext == nullptr)
		{
			mNode++;
			mAtEnd = true;
		}
		else
		{
			mNode = mNode->mNext;
		}
		return *this; 
	}

	iterator& operator -- () 
	{ 
		ThrowIfNull();
		if (mAtEnd)
		{
			mNode--;
			mAtEnd = false;
		}
		else
		{
			mNode = mNode->mPrev; 
		}		
		return *this; 
	}
	
	bool operator == (const iterator& other) const 
	{
		if (mAtEnd) return other.mAtEnd;
		return mNode == other.mNode; 
	}
	
	bool operator != (const iterator& other) const 
	{ 
		if (mAtEnd) return !other.mAtEnd;
		if (other.mAtEnd) return true;
		return mNode != other.mNode; 
	}
	
	T& operator * () { ThrowIfNull(); return mNode->mItem; }

	// read about iterator traits and category tags later...
};

template<typename T>
typename Deque<T>::iterator Deque<T>::begin()
{
	return std::move(iterator(mFirstItem));
}

template<typename T>
typename Deque<T>::iterator Deque<T>::end()
{
	return std::move(iterator(mLastItem + 1, true));
}

} /* namespace containers */
} /* namespace mabz */