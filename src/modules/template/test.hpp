#ifndef __TEMPLATE_TEST_H__
#define __TEMPLATE_TEST_H__

#include <type_traits>

#include "tools/DebugLog.h"
namespace test
{
    template<typename T>
    T add(const T& a, const T& b)
    {
        DBG_LOGD("hello, add!");
        return a + b;
    }


    template<template <typename> class T1, typename T2>
    struct Func_1
    {
        using type = typename T1<T2>::type;
    };

    template <template<typename> class T1, typename T2>
    using Fun = typename Func_1<T1, T2>::type;


    template <bool AddOrRemoveRef> struct Func_2;

    template<>
    struct Func_2<true>
    {
        template <typename T>
        using type = std::add_lvalue_reference<T>;
    };

    template<>
    struct Func_2<false>
    {
        template <typename T>
        using type = std::remove_reference<T>;
    };



    template <typename T>
    struct RemoveReferenceConst_
    {
    private:
        using inter_type = typename std::remove_reference<T>::type;
    public:
        using type = typename std::remove_const<inter_type>::type;
    };

    template <typename T>
    using RemoveReferenceConst = typename RemoveReferenceConst_<T>::type;

    template <typename TW>
    struct Wrapper
    {
        template <typename T, typename TDummy = void>
        struct fun_
        {
            constexpr static size_t value = 0;
        };

        template <typename TDummy>
        struct fun_<int, TDummy>
        {
            constexpr static size_t value = 1;
        };
    };

    template <size_t Input>
    constexpr size_t OnesCount = (Input % 2) + OnesCount<Input / 2>;

    template <> constexpr size_t OnesCount<0> = 0;
}
#endif
