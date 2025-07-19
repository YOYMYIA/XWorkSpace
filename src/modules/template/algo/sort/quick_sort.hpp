#ifndef ALGORITHM_SORT_QUICK_SORT_HPP
#define ALGORITHM_SORT_QUICK_SORT_HPP
#include <assert.h>

namespace algo
{

template<typename Iterator, typename CompareType = std::less<typename std::iterator_traits<Iterator>::value_type>>
void quick_sort(Iterator begin, Iterator end, CompareType compare = CompareType())
{
    auto size = std::distance(begin, end);
    if(size <= 1)
    {
        return;
    }


}

}

#endif