//
// Created by xlinux on 25-7-9.
//

#ifndef UT__TEST_HPP__
#define UT__TEST_HPP__

#include "template/test.hpp"
#include <gtest/gtest.h>
#include "template/MetaNN/VarTypeDict.h"
#include "policy.hpp"
#include <math.h>

using namespace MetaNN;

struct A;
struct B;
struct Weight;
struct XX;

struct FParams : public VarTypeDict<struct A,
                                    struct B,
                                    struct Weight>
{
};

template <typename TIn>
float fun(const TIn &in)
{
    auto a = in.template Get<A>();
    auto b = in.template Get<B>();
    auto weight = in.template Get<Weight>();

    return a * weight + b * (1 - weight);
}

class Test_ : public ::testing::Test
{
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(Test_, OneTest)
{
    std::cout << "Test named params...\t";
    auto res = fun(FParams::Create()
                       .Set<A>(1.3f)
                       .Set<B>(2.4f)
                       .Set<Weight>(0.1f));
    EXPECT_LE(fabs(res - 0.1 * 1.3 - 0.9 * 2.4), 0.0001);
    std::cout << "done\n";
}

#endif
