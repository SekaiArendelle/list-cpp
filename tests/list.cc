#include <cassert>
#include <list/list.hh>

static_assert(::list::details::allocator<::std::allocator<int>>);

namespace list_unittests {

constexpr void test_construct() {
    auto list = ::list::List<int>{};
    assert(list.is_empty());
    assert(!list.has_single());
    assert(!list.has_multiple());
    list.push_front(1);
    assert(!list.is_empty());
    assert(list.has_single());
    assert(!list.has_multiple());
    list.push_front(2);
    assert(!list.is_empty());
    assert(!list.has_single());
    assert(list.has_multiple());
}

}

auto main() -> int {
    ::list_unittests::test_construct();

    return 0;
}
