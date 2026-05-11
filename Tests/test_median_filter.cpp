#include "MedianFilter.hpp"
#include <cassert>

int main()
{
    MedianFilter filter;

    assert(!filter.initialized());

    assert(filter.apply(10) == 10);
    assert(!filter.initialized());

    filter.seed(25);
    assert(filter.initialized());

    // 17 tane 25 + 1 tane 1000 olunca median hâlâ 25 olmalı
    assert(filter.apply(1000) == 25);

    filter.reset();
    assert(!filter.initialized());

    assert(filter.apply(5) == 5);
    assert(filter.apply(1) == 5);
    assert(filter.apply(9) == 5);

    return 0;
}