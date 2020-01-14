#include <gtest/gtest.h>

#include <algo_lib/exceptions.h>
#include <algo_lib/percolation.h>

namespace {

namespace nsperc = mabz::percolation;

TEST(PercolationTest, TestConstructorThrows)
{
	ASSERT_THROW(
		[]() { nsperc::Percolation p(-5); }(), 
		mabz::IllegalArgumentException);
}

TEST(PercolationFixtureless, TestMethodsThrow)
{
	nsperc::Percolation p(5);
	
	ASSERT_THROW(p.Open(0, 3), mabz::IllegalArgumentException);
	ASSERT_THROW(p.Open(1, 8), mabz::IllegalArgumentException);
	ASSERT_THROW(p.IsOpen(-2, 5), mabz::IllegalArgumentException);
	ASSERT_THROW(p.IsOpen(2, -5), mabz::IllegalArgumentException);
	ASSERT_THROW(p.IsFull(9, 1), mabz::IllegalArgumentException);
	ASSERT_THROW(p.IsFull(1, 9), mabz::IllegalArgumentException);
}

TEST(PercolationTest, TestNoPercolateCaseWithSomeOpenings)
{
	const int n{5};
	nsperc::Percolation p(n);
	
	p.Open(3, 2);
	p.Open(1, 5);

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if ((i == 3 && j == 2) || (i == 1 && j == 5))
			{
				ASSERT_TRUE(p.IsOpen(i, j));
			}
			else
			{
				ASSERT_FALSE(p.IsOpen(i, j));
			}
		}
	}

	ASSERT_EQ(p.GetNumberOfOpenSites(), 2);
	
	ASSERT_FALSE(p.DoesPercolate());
	
	ASSERT_FALSE(p.IsFull(3, 2));
	ASSERT_FALSE(p.IsFull(3, 3));
	ASSERT_TRUE(p.IsFull(1, 5));
}

TEST(PercolationTest, TestPercolateCase)
{
	nsperc::Percolation p(5);
	p.Open(1, 1);
	p.Open(2, 1);
	p.Open(2, 2);
	p.Open(3, 2);
	p.Open(3, 1);
	p.Open(3, 5);
	p.Open(4, 1);
	p.Open(4, 2);
	p.Open(4, 3);
	p.Open(4, 4);
	p.Open(5, 4);

	ASSERT_EQ(p.GetNumberOfOpenSites(), 11);
	
	ASSERT_TRUE(p.DoesPercolate());
	
	ASSERT_TRUE(p.IsFull(3, 2));
	ASSERT_FALSE(p.IsFull(5, 1));
}

} /* anon namespace */