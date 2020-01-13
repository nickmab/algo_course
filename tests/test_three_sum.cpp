#include <vector>

#include <gtest/gtest.h>

#include <algo_lib/three_sum.h>

namespace {

namespace nssearch = mabz::search;

TEST(ThreeSumTest, TestNothing)
{
	std::vector<int> vec{5, 2, 3, 1, 9, 7};
	nssearch::ThreeSum ts(0, vec);
	ts.Run();
}

} /* anon namespace */