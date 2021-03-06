#include <exception>
#include <gtest/gtest.h>

#include <algo_lib/union_find.h>

namespace {

TEST(UnionFindTest, TestConnected)
{
	mabz::UnionFind uf(9);
	uf.Union(1, 2);
	uf.Union(3, 4);
	uf.Union(3, 4);
	uf.Union(3, 5);
	uf.Union(6, 7);
	uf.Union(7, 8);

	EXPECT_TRUE(uf.Connected(1, 2));

	EXPECT_TRUE(uf.Connected(3, 4));
	EXPECT_TRUE(uf.Connected(3, 5));
	EXPECT_TRUE(uf.Connected(4, 5));
	
	EXPECT_TRUE(uf.Connected(6, 7));
	EXPECT_TRUE(uf.Connected(7, 8));
	EXPECT_TRUE(uf.Connected(6, 8));

	EXPECT_FALSE(uf.Connected(1, 3));
	EXPECT_FALSE(uf.Connected(2, 3));
	
	EXPECT_FALSE(uf.Connected(1, 4));
	EXPECT_FALSE(uf.Connected(2, 4));
	EXPECT_FALSE(uf.Connected(1, 5));
	EXPECT_FALSE(uf.Connected(2, 5));
	
	EXPECT_FALSE(uf.Connected(1, 6));
	EXPECT_FALSE(uf.Connected(1, 7));
	EXPECT_FALSE(uf.Connected(1, 8));
	EXPECT_FALSE(uf.Connected(2, 6));
	EXPECT_FALSE(uf.Connected(2, 7));
	EXPECT_FALSE(uf.Connected(2, 8));

	EXPECT_FALSE(uf.Connected(3, 6));
	EXPECT_FALSE(uf.Connected(3, 7));
	EXPECT_FALSE(uf.Connected(3, 8));
	EXPECT_FALSE(uf.Connected(4, 6));
	EXPECT_FALSE(uf.Connected(4, 7));
	EXPECT_FALSE(uf.Connected(4, 8));
	EXPECT_FALSE(uf.Connected(5, 6));
	EXPECT_FALSE(uf.Connected(5, 7));
	EXPECT_FALSE(uf.Connected(5, 8));
}

TEST(UnionFindTest, TestUnionToSelf)
{
	mabz::UnionFind uf(4);
	uf.Union(1, 1);
	uf.Union(2, 2);
	uf.Union(3, 3);
	uf.Union(2, 2);
	uf.Union(2, 3);

	EXPECT_TRUE(uf.Connected(1, 1));
	EXPECT_TRUE(uf.Connected(2, 3));
	EXPECT_TRUE(uf.Connected(3, 2));
	
	EXPECT_FALSE(uf.Connected(1, 2));
	EXPECT_FALSE(uf.Connected(1, 3));
	EXPECT_FALSE(uf.Connected(2, 1));
	EXPECT_FALSE(uf.Connected(3, 1));
}

TEST(UnionFindTest, TestConnectedShouldThrow)
{
	mabz::UnionFind uf(3);
	uf.Union(1, 2);

	// look for out of bounds value -1
	ASSERT_THROW(uf.Connected(-1, 1), std::exception);
	// look for out of bounds value 3
	ASSERT_THROW(uf.Connected(1, 3), std::exception);
}

TEST(UnionFindTest, TestReset)
{
	mabz::UnionFind uf(9);
	uf.Union(1, 2);
	uf.Union(3, 4);
	uf.Union(3, 4);
	uf.Union(3, 5);
	uf.Union(6, 7);
	uf.Union(7, 8);

	EXPECT_TRUE(uf.Connected(1, 2));
	EXPECT_TRUE(uf.Connected(6, 8));
	uf.Reset();
	EXPECT_FALSE(uf.Connected(1, 2));
	EXPECT_FALSE(uf.Connected(6, 8));

	uf.Union(1, 2);
	uf.Union(2, 4);
	EXPECT_TRUE(uf.Connected(1, 4));
}

} /* anonymous namespace */