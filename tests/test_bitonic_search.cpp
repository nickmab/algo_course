#include <cmath>
#include <exception>
#include <functional>
#include <vector>
#include <array>

#include <gtest/gtest.h>

#include <algo_lib/bitonic_search.h>

namespace {

namespace nssearch = mabz::search;

TEST(FindBitonicTurningPointTest, TestDefaultCases)
{
	{
		std::vector<int> v{1, 2, 1};
		ASSERT_EQ(nssearch::FindBitonicTurningPoint<int>(v), 1);
	}

	{
		std::vector<double> v{-1, 2, 3, 4, 3};
		ASSERT_EQ(nssearch::FindBitonicTurningPoint<double>(v), 3);
	}

	{
		std::vector<int> v{5, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::FindBitonicTurningPoint<int>(v), 0);
	}

	{
		std::vector<int> v{1, 2, 3, 4};
		ASSERT_EQ(nssearch::FindBitonicTurningPoint<int>(v), 3);
	}

	{
		std::vector<int> v{1, 2, 3, 4, -99};
		ASSERT_EQ(nssearch::FindBitonicTurningPoint<int>(v), 3);
	}

	{
		std::array<int, 6> v{1, 2, 10, 22, 20, 9};
		ASSERT_EQ(nssearch::FindBitonicTurningPoint<int>(v), 3);
	}
}

TEST(FindBitonicTurningPointTest, TestCustomComparisonWithDoubles)
{
	std::array<double, 5> a{1, 3.06, 3.05, 3.04, 4};
	std::function<bool(double,double)> func = [](double a, double b) ->bool {
		return abs(a-b) < 0.1;
	};

	// should avoid being confused by the 3.06, 5 and 4 in the middle.
	ASSERT_EQ(nssearch::FindBitonicTurningPoint<double>(a, func), 4);
}

TEST(BitonicSearchTest, TestDefaultCases)
{
	{
		std::vector<int> v{1, 2, -1};
		ASSERT_EQ(nssearch::BitonicSearch<int>(1, v), 0);
	}

	{
		std::vector<int> v{1, 2, -1};
		ASSERT_EQ(nssearch::BitonicSearch<int>(2, v), 1);
	}

	{
		std::vector<int> v{1, 2, -1};
		ASSERT_EQ(nssearch::BitonicSearch<int>(-1, v), 2);
	}

	// check monotonic cases
	{
		std::vector<int> v{5, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::BitonicSearch<int>(2, v), 3);
	}

	{
		std::vector<int> v{1, 2, 3, 4, 5};
		ASSERT_EQ(nssearch::BitonicSearch<int>(2, v), 1);
	}

	{
		std::vector<int> v{1, 2, 3, 4, 2, 2};
		ASSERT_EQ(nssearch::BitonicSearch<int>(4, v), 3);
	}

	// check the duplicates don't trip it up...
	{
		std::vector<int> v{1, 2, 3, 3, 3, 3, 3, 3, 3};
		ASSERT_NE(nssearch::BitonicSearch<int>(3, v), -1);
	}

	{
		std::vector<int> v{1, 2, 3, 3, 3, 3, 3, 3, 1};
		ASSERT_NE(nssearch::BitonicSearch<int>(1, v), -1);
	}

	{
		std::vector<int> v{2, 2, 3, 3, 3, 3, 3, 1};
		ASSERT_NE(nssearch::BitonicSearch<int>(1, v), -1);
	}

	// test when the values aren't there...
	{
		std::vector<int> v{2, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::BitonicSearch<int>(7, v), -1);
	}

	// test when the values aren't there...
	{
		std::vector<int> v{2, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::BitonicSearch<int>(7, v), -1);
	}
}

TEST(BitonicSearchTest, TestCustomComparisonWithDoubles)
{
	std::function<bool(double,double)> func = [](double a, double b) ->bool {
		return abs(a-b) < 0.1;
	};

	{
		std::array<double, 6> a{1, 3.06, 3.05, 3.04, 2.4};
		ASSERT_NE(nssearch::BitonicSearch<double>(3.05, a, func), -1);
	}

	{
		std::array<double, 6> a{1, 3.06, 3.05, 3.04, 2.4};
		ASSERT_EQ(nssearch::BitonicSearch<double>(1.09, a, func), 0);
	}

	{
		std::array<double, 6> a{1, 3.06, 3.05, 3.04, 2.4};
		// it returns the left-most value considered to be "equal" here.
		ASSERT_EQ(nssearch::BitonicSearch<double>(5.0, a, func), -1);
	}
}

TEST(BetterBitonicSearchTest, TestDefaultCases)
{
	{
		std::vector<int> v{1, 2, -1};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(1, v), 0);
	}

	{
		std::vector<int> v{1, 2, -1};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(2, v), 1);
	}

	{
		std::vector<int> v{1, 2, -1};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(-1, v), 2);
	}

	// check monotonic cases
	{
		std::vector<int> v{5, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(2, v), 3);
	}

	{
		std::vector<int> v{1, 2, 3, 4, 5};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(2, v), 1);
	}

	{
		std::vector<int> v{1, 2, 3, 4, 2, 2};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(4, v), 3);
	}

	// check the duplicates don't trip it up...
	{
		std::vector<int> v{1, 2, 3, 3, 3, 3, 3, 3, 3};
		ASSERT_NE(nssearch::BetterBitonicSearch<int>(3, v), -1);
	}

	{
		std::vector<int> v{1, 2, 3, 3, 3, 3, 3, 3, 1};
		ASSERT_NE(nssearch::BetterBitonicSearch<int>(1, v), -1);
	}

	{
		std::vector<int> v{2, 2, 3, 3, 3, 3, 3, 1};
		ASSERT_NE(nssearch::BetterBitonicSearch<int>(1, v), -1);
	}

	// test when the values aren't there...
	{
		std::vector<int> v{2, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(7, v), -1);
	}

	// test when the values aren't there...
	{
		std::vector<int> v{2, 4, 3, 2, 1};
		ASSERT_EQ(nssearch::BetterBitonicSearch<int>(7, v), -1);
	}
}

TEST(BetterBitonicSearchTest, TestCustomComparisonWithDoubles)
{
	std::function<bool(double,double)> func = [](double a, double b) ->bool {
		return abs(a-b) < 0.1;
	};

	{
		std::array<double, 6> a{1, 3.06, 3.05, 3.04, 2.4};
		ASSERT_NE(nssearch::BetterBitonicSearch<double>(3.05, a, func), -1);
	}

	{
		std::array<double, 6> a{1, 3.06, 3.05, 3.04, 2.4};
		ASSERT_EQ(nssearch::BetterBitonicSearch<double>(1.09, a, func), 0);
	}

	{
		std::array<double, 6> a{1, 3.06, 3.05, 3.04, 2.4};
		// it returns the left-most value considered to be "equal" here.
		ASSERT_EQ(nssearch::BetterBitonicSearch<double>(5.0, a, func), -1);
	}
}

} /* anon namespace */