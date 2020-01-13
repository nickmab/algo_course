#pragma once

#include <functional>
#include <sstream>
#include <string>

#include <algo_lib/binary_search.h>
#include <algo_lib/exceptions.h>

namespace mabz { namespace search {

// Look for value "target" in bitonic container "cont" specifically between 
// container indices [begin, end] inclusive. Can optionally provide an equality 
// comparison function (e.g. for comparing doubles), otherwise defaults 
// to using == operator. Return -1 if not found.
// template <typename T, typename Container>
// int BinSearch(const T& target, const Container& cont,
// 	std::function<bool(T,T)> eqFunc=[](T a, T b)->bool{ return a == b; })
// {

} /* namespace search */
} /* namespace mabz */
