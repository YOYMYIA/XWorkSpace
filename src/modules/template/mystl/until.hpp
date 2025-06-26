#ifndef MYSTL_UNTIL_H__
#define MYSTL_UNTIL_H__
#include "template/base.h"
#include "type_traits.hpp"
#include <cstddef>

namespace myTpl
{
    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(args);
    }

}

#endif // MYSTL_UNTIL_H__