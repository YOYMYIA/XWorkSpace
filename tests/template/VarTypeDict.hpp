#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

namespace test{

struct NullParameter{};

template <typename T>
struct Identity_
{
    using type = T;
};

//元函数create_, 传入参数,获取它的返回类型,使用该类型构造一个对象并返回
/**
 * N 表示还需要构造的元素数量
 * TCont 表示容器类型,用来存储最终的结果(值的类型是数组)
 * T是已生成的类型序列
 * 
 * 在其内部构造一个NullParamter类型并放到类型数组中,将N-1,进行下一次迭代
 */
template <size_t N, template <typename...> class TCont, typename ...T>
struct Create_
{
    using type = typename Create_<N - 1, TCont, NullParameter, T...>::type;

};

/**
 * create_特化版本,在N=0 时,循环终止,系统直接返回TCont
 */
template <template <typename...> class TCont, typename...T>
struct Create_<0, TCont, T...>
{
    using type = TCont<T...>;
};

/// ====================== FindTagPos ===================================
/**
 * 主模板 (递归) 当TCurTag不是TFindTag时,递归处理剩余类型
 * TTags表示剩余处理类型
 * 
 * 递归逻辑:移除当前TCurTag类型, 将剩余类型进行下一次递归
 */
template <typename TFindTag, size_t N, typename TCurTag, typename...TTags>
struct Tag2ID_
{
    constexpr static size_t value = Tag2ID_<TFindTag, N + 1, TTags...>::value;
};

/**
 * 特化模板(终止条件) 
 * 当前类型和目标类型相同时,返回当前索引
 */
template <typename TFindTag, size_t N, typename...TTags>
struct Tag2ID_<TFindTag, N, TFindTag, TTags...>
{
    constexpr static size_t value = N;
};

/**
 * 模板变量(接口)
 */
template <typename TFindTag, typename...TTags>
constexpr size_t Tag2ID = Tag2ID_<TFindTag, 0, TTags...>::value;

/// ====================== NewTupleType ===================================
/**
 * 主模板声明(未定义) 处理通用情况
 * @param TVal 要插入的新类型
 * @param N 目标插入的位置
 * @param M 当前处理位置
 * @param TProcessedTypes 已处理了类型容器集合
 * @param 剩余待处理类型 
 */
template <typename TVal, size_t N, size_t M, typename TProcessedTypes, typename... TRemainTypes>
struct NewTupleType_;

template <typename TVal, size_t N, size_t M, template <typename...> class TCont,
          typename...TModifiedTypes, typename TCurType, typename... TRemainTypes>
struct NewTupleType_<TVal, N, M, TCont<TModifiedTypes...>,
                     TCurType, TRemainTypes...>
{
    using type = typename NewTupleType_<TVal, N, M + 1,
                                        TCont<TModifiedTypes..., TCurType>,
                                        TRemainTypes...>::type;
};

template <typename TVal, size_t N, template <typename...> class TCont,
          typename...TModifiedTypes, typename TCurType, typename... TRemainTypes>
struct NewTupleType_<TVal, N, N, TCont<TModifiedTypes...>, TCurType, TRemainTypes...>
{
    using type = TCont<TModifiedTypes..., TVal, TRemainTypes...>;
};

template <typename TVal, size_t TagPos, typename TCont, typename... TRemainTypes>
using NewTupleType = typename NewTupleType_<TVal, TagPos, 0, TCont, TRemainTypes...>::type;

/// ====================== Pos2Type ===================================
template <size_t Pos, typename...TTags>
struct Pos2Type_
{
    static_assert((Pos != 0), "Invalid position.");
};

template <size_t Pos, typename TCur, typename...TTags>
struct Pos2Type_<Pos, TCur, TTags...>
{
    using type = typename std::conditional_t<(Pos == 0),
                                             Identity_<TCur>,
                                             Pos2Type_<Pos-1, TTags...>>::type;
};

template <size_t Pos, typename...TTags>
using Pos2Type = typename Pos2Type_<Pos, TTags...>::type;

/// ====================== VarType ===================================
template <typename ...TParameters>
struct VarTypeDict
{
    //类型
    //values和核心是存储区域是一个智能指针数组
    //其中每一个元素都是一个智能指针
    template <typename ...TTypes>
    struct Values
    {
        Values() = default;

        //input 是一个右值引用的数组,数据类型是shared_ptr
        Values(std::shared_ptr<void>(&& input)[sizeof...(TTypes)])
        {
            for(size_t i = 0; i < sizeof...(TTypes); ++i)
            {
                m_tuple[i] = std::move(input[i]);
            } 
        }

    public:
        //set函数后面跟&&表示该函数只能用于右值
        /**
         * @param TTag 表示键
         * @param TVal 表示值的类型
         * x.set<A>(true) 根据C++自动推导规则,只需要显示指定第一个模板参数类型,编译器自动昂推导第二个
         */

        template <typename TTag, typename TVal>
        auto Set(TVal&& val) &&
        {
            //获取类型的序号
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;

            //使用是std::decay 函数,去除参数的const 和引用等修饰符
            using rawVal = std::decay_t<TVal>;
            //构造该类型的一个实例
            rawVal* tmp = new rawVal(std::forward<TVal>(val));

            m_tuple[TagPos] = std::shared_ptr<void>(tmp, 
                [](void* ptr){
                    //析构的方式
                    rawVal* nptr = static_cast<rawVal* >(ptr);
                    delete nptr;
                }
            );

            //获取新类型,并为该类型构造新对象并返回
            using new_type = NewTupleType<rawVal, TagPos, Values<>, TTypes...>;
            return new_type(std::move(m_tuple));
        }
        
        template <typename TTag>
        const auto& Get() const
        {
            constexpr static size_t TagPos = Tag2ID<TTag, TParameters...>;
 
            using AimType = Pos2Type<TagPos, TTypes...>;

            void* tmp = m_tuple[TagPos].get();
            AimType* res = static_cast<AimType*>(tmp);
            return *res;
        }

        template <typename TTag>
        using ValueType = Pos2Type<Tag2ID<TTag, TParameters...>, TTypes...>;



    private:
        std::shared_ptr<void> m_tuple[sizeof...(TTypes)];
    };

public:

/**
 * Create 调用Create_函数, 传入 TParameters参数大小,同时传入数组容器Values
 * Values与Create均定义在VarTypeDict内部,在Create中使用Values时,无需指定外围的VarTypeDict
 * 此时的Create_中的T...对应一个空类型序列
 */
    static auto Create()
    {
        using type = typename Create_<sizeof...(TParameters), Values>::type;
        return type();
    }


};



}