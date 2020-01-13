#include <array>
#include <cmath>
#include <functional>
#include <vector>

#include <gtest/gtest.h>

#include <algo_lib/three_sum.h>

namespace {

namespace nssearch = mabz::search;

TEST(ThreeSumTest, TestBasicCase)
{
	std::vector<int> vec{5, 2, 3, 1, 9, 7};
	nssearch::ThreeSum ts(10, vec);
	ts.Run();
	
	// should find 1,2,7 and 2,3,5 in that order due to internal sorting.
	ASSERT_EQ(ts.GetNumberOfTriplets(), 2);
	auto results = ts.GetResults();
	auto result1 = results[0];
	auto result2 = results[1];
	ASSERT_EQ(result1.first, 1);
	ASSERT_EQ(result1.second, 2);
	ASSERT_EQ(result1.third, 7);
	ASSERT_EQ(result2.first, 2);
	ASSERT_EQ(result2.second, 3);
	ASSERT_EQ(result2.third, 5);
}

TEST(ThreeSumTest, TestCustomFuncCase)
{
	std::array<double, 5> a{5.0, -5.0, 1.2, 0.0, 3.7};
	std::function<bool(double,double)> func = [](double a, double b) ->bool {
		return std::fabs(a-b) < 0.2;
	};
	nssearch::ThreeSum ts(0.0, a, func);
	ts.Run();
	
	// should find -5.0, 0.0, 5.0 and -5.0, 1.2, 3.7 in that order due to internal sorting.
	ASSERT_EQ(ts.GetNumberOfTriplets(), 2);
	auto results = ts.GetResults();
	auto result1 = results[0];
	auto result2 = results[1];
	ASSERT_DOUBLE_EQ(result1.first, -5.0);
	ASSERT_DOUBLE_EQ(result1.second, 0.0);
	ASSERT_DOUBLE_EQ(result1.third, 5.0);
	ASSERT_DOUBLE_EQ(result2.first, -5.0);
	ASSERT_DOUBLE_EQ(result2.second, 1.2);
	ASSERT_DOUBLE_EQ(result2.third, 3.7);
}

} /* anon namespace */