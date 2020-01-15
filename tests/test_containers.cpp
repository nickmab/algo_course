#include <exception>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include <algo_lib/containers.h>

namespace {

namespace nscont = mabz::containers;

TEST(DequeTest, TestDefaultCase)
{
	nscont::Deque<int> d;
	ASSERT_EQ(d.size(), 0);
	ASSERT_TRUE(d.empty());

	d.push_front(7);
	ASSERT_EQ(d.size(), 1);
	ASSERT_FALSE(d.empty());

	{
		int x = d.pop_back();
		ASSERT_EQ(x, 7);
	}

	ASSERT_EQ(d.size(), 0);
	ASSERT_TRUE(d.empty());

	d.push_back(8);
	ASSERT_EQ(d.size(), 1);
	ASSERT_FALSE(d.empty());

	{
		int x = d.pop_front();
		ASSERT_EQ(x, 8);
	}
	ASSERT_EQ(d.size(), 0);
	ASSERT_TRUE(d.empty());

	{
		std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
		for (auto& i : v)
		{
			// this should result in a deque containing..
			// 14, 12, 10, 8, 6, 4, 2, 1, 3, 5, 7, 9, 11, 13
			if (i % 2 == 0)
			{
				d.push_front(i);
			}
			else
			{
				d.push_back(i);
			}
		}
	}
	
	{
		std::vector<int> v;
		bool front = true;
		while (!d.empty())
		{
			// this should result in basically a reverse sorted vector from where we started!
			if (front)
			{
				v.push_back(d.pop_front());
			}
			else
			{
				v.push_back(d.pop_back());
			}
			front = !front;
		}
		
		for (int i = 0; i < v.size(); ++i)
		{
			ASSERT_EQ(v[i], 14-i);
		}
	}
}

TEST(DequeTest, TestIterator)
{
	nscont::Deque<int> d;

	std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	for (auto& i : v)
	{
		// this should result in a deque containing..
		// 14, 12, 10, 8, 6, 4, 2, 1, 3, 5, 7, 9, 11, 13
		if (i % 2 == 0)
		{
			d.push_front(i);
		}
		else
		{
			d.push_back(i);
		}
	}

	std::vector<int> expectedResult{
		14, 12, 10, 8, 6, 4, 2, 1, 3, 5, 7, 9, 11, 13};
	
	{
		std::vector<int> gotResult;
		
		nscont::Deque<int>::iterator it;
		for (it = d.begin(); it != d.end(); ++it)
		{
			gotResult.push_back(*it);
		}
	
		ASSERT_EQ(expectedResult, gotResult);
	}


	{
		std::vector<int> gotResult;
		
		for (auto& it : d)
		{
			gotResult.push_back(it);
		}
	
		ASSERT_EQ(expectedResult, gotResult);
	}
}

} /* anon namespace */