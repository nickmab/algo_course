#include <cmath>
#include <exception>
#include <functional>
#include <vector>
#include <array>

#include <gtest/gtest.h>

#include <algo_lib/binary_search.h>

namespace {

namespace nssearch = mabz::search;

TEST(BinSearchTest, TestDefaultCase)
{
	std::vector<int> v{1, 2, 3, 6, 7, 8};
	ASSERT_EQ(nssearch::BinSearch(1, v, 0, 5), 0);
	ASSERT_EQ(nssearch::BinSearch(1, v, 1, 5), -1);
	ASSERT_EQ(nssearch::BinSearch(2, v, 0, 5), 1);
	ASSERT_EQ(nssearch::BinSearch(8, v, 1, 5), 5);
	ASSERT_EQ(nssearch::BinSearch(8, v, 1, 4), -1);
	ASSERT_EQ(nssearch::BinSearch(9, v, 0, 5), -1);
	ASSERT_EQ(nssearch::BinSearch(3, v, 2, 2), 2);
}

TEST(BinSearchTest, TestWorksWithArray)
{
	std::array<int, 6> a{1, 2, 3, 6, 7, 8};
	ASSERT_EQ(nssearch::BinSearch(1, a, 0, 5), 0);
	ASSERT_EQ(nssearch::BinSearch(1, a, 1, 5), -1);
	ASSERT_EQ(nssearch::BinSearch(2, a, 0, 5), 1);
	ASSERT_EQ(nssearch::BinSearch(8, a, 1, 5), 5);
	ASSERT_EQ(nssearch::BinSearch(8, a, 1, 4), -1);
	ASSERT_EQ(nssearch::BinSearch(9, a, 0, 5), -1);
}

TEST(BinSearchTest, TestReverseSort)
{
	std::vector<int> v{8, 7, 6, 3, 2, 1};
	ASSERT_EQ(nssearch::ReverseBinSearch(1, v, 0, 5), 5);
	ASSERT_EQ(nssearch::ReverseBinSearch(1, v, 1, 5), 5);
	ASSERT_EQ(nssearch::ReverseBinSearch(2, v, 0, 5), 4);
	ASSERT_EQ(nssearch::ReverseBinSearch(8, v, 1, 5), -1);
	ASSERT_EQ(nssearch::ReverseBinSearch(8, v, 0, 4), 0);
	ASSERT_EQ(nssearch::ReverseBinSearch(9, v, 0, 5), -1);
	ASSERT_EQ(nssearch::ReverseBinSearch(3, v, 3, 3), 3);
}

TEST(BinSearchTest, TestCustomComparisonWithDoubles)
{
	std::array<double, 4> a{1, 2, 3, 6};
	std::function<bool(double,double)> func = [](double a, double b) ->bool {
		return abs(a-b) < 0.1;
	};

	ASSERT_EQ(nssearch::BinSearch(1.09, a, 0, 3, func), 0);
	ASSERT_EQ(nssearch::BinSearch(1.0, a, 1, 3, func), -1);
	ASSERT_EQ(nssearch::BinSearch(1.99, a, 0, 3, func), 1);
	ASSERT_EQ(nssearch::BinSearch(6.0, a, 1, 3, func), 3);
	ASSERT_EQ(nssearch::BinSearch(6.0, a, 1, 2, func), -1);
}

TEST(BinSearchTest, TestThrowingArgs)
{
	std::vector<int> v{1, 2, 3, 6, 7, 8};
	ASSERT_THROW(nssearch::BinSearch(1, v, 1, 0), std::exception);
	ASSERT_THROW(nssearch::BinSearch(1, v, 0, 10), std::exception);
	ASSERT_THROW(nssearch::BinSearch(1, v, -1, 5), std::exception);
}

} /* anon namespace */