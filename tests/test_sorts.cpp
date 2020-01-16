#include <vector>

#include <gtest/gtest.h>

#include <algo_lib/sorts.h>

namespace {

namespace nssort = mabz::sorts;

TEST(DutchFlagSortTest, TestDefaultCase)
{
	#define r nssort::RWB::RED
	#define w nssort::RWB::WHITE
	#define b nssort::RWB::BLUE
	
	{ // some "random" order...
		std::vector<nssort::RWB> v{
			b,w,b,w,r,w,r,r,b,r,r,w
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			r,r,r,r,r,w,w,w,w,b,b,b
		};
		ASSERT_EQ(v, expected);
	}

	{ // already BRW sorted - not quite reverse.
		std::vector<nssort::RWB> v{
			b,b,b,b,r,r,r,r,w,w,w,w
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			r,r,r,r,w,w,w,w,b,b,b,b
		};
		ASSERT_EQ(v, expected);
		ASSERT_LE(loopsTaken, v.size()); // pretty sure should always be ==
	}

	{ // already reverse sorted.
		std::vector<nssort::RWB> v{
			b,b,b,b,w,w,w,w,r,r,r,r
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			r,r,r,r,w,w,w,w,b,b,b,b
		};
		ASSERT_EQ(v, expected);
		ASSERT_LE(loopsTaken, v.size()); // pretty sure should always be ==
	}

	{ // already sorted
		std::vector<nssort::RWB> v{
			r,r,r,r,w,w,w,w,b,b,b,b
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			r,r,r,r,w,w,w,w,b,b,b,b
		};
		ASSERT_EQ(v, expected);
		ASSERT_LE(loopsTaken, v.size()); // pretty sure should always be ==
	}

	{ // all red?
		std::vector<nssort::RWB> v{
			r,r,r,r,r,r,r
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			r,r,r,r,r,r,r
		};
		ASSERT_EQ(v, expected);
		ASSERT_LE(loopsTaken, v.size()); // pretty sure should always be ==
	}

	{ // all white?
		std::vector<nssort::RWB> v{
			w,w,w,w,w,w,w
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			w,w,w,w,w,w,w
		};
		ASSERT_EQ(v, expected);
		ASSERT_LE(loopsTaken, v.size()); // pretty sure should always be ==
	}

	{ // all blue?
		std::vector<nssort::RWB> v{
			b,b,b,b,b,b,b
		};
		
		int loopsTaken = nssort::DutchFlagSort(v);
	
		std::vector<nssort::RWB> expected{
			b,b,b,b,b,b,b
		};
		ASSERT_EQ(v, expected);
		ASSERT_LE(loopsTaken, v.size()); // pretty sure should always be ==
	}

	#undef r
	#undef w
	#undef b
}

} /* anon namespace */