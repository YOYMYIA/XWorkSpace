#ifndef T__TYPE_TRAITS_H__
#define T__TYPE_TRAITS_H__
#include "../base.h"

#include <type_traits>


namespace myTpl
{

template <class T, T v>
struct m_integral_constant
{
    static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;

/******************************************************************/

template <class T1, class T2>
struct pair{};

template <class T>
struct is_pair : myTpl::m_false_type{};

template <class T1, class T2>
struct is_pair<myTpl::pair<T1, T2>> : myTpl::m_true_type{};

}//mytpl

#endif
