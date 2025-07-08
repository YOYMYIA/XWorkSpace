#ifndef __MYSTL_UNTIL_HPP__
#define __MYSTL_UNTIL_HPP__
#include "template/base.h"
#include <cstddef>
#include "type_traits.hpp"

namespace mystl
{
    template <class T>
    typename std::remove_reference<T>::type&& move(T && arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept
    {
        return static_cast<T&&> (arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
    {
        return static_cast<T&&> (arg);
    }

    template <class T>
    void swap(T& lhs, T& rhs)
    {
        auto tmp(mystl::move(lhs));
        lhs = mystl::move(rhs);
        rhs = mystl::move(tmp);
    }

    template <class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
    {
        for (; first1 != last1; ++first1, (void) ++first2)
        {
            mystl::swap(*first1, *first2);
        }
        return first2;
    }

    template <class Tp, size_t N>
    void swap(Tp(&a)[N], Tp(&b)[N])
    {
        mystl::swap_range(a, a + N, b);
    }

    template <class Ty1, class Ty2>
    struct pair
    {
        typedef Ty1 first_type;
        typedef Ty2 second_type;
        first_type first;
        second_type second;

        template <class Other1 = Ty1, class Other2 = Ty2,
            typename = typename std::enable_if<
            std::is_default_constructible<Other1>::value &&
            std::is_default_constructible<Other2>::value , void>::type>
            constexpr pair()
            : first(), second()
        {
        }

        template <class U1 = Ty1, class U2 = Ty2,
        typename std::enable_if<
            std::is_default_constructible<U1>::value &&
            std::is_default_constructible<U2>::value , void>::type = 0>
            constexpr pair(const Ty1& a, const Ty2& b)
            : first(a), second(b)
        {
        }
    };


} // namespace mystl

#endif