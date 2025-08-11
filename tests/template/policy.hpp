#pragma once

namespace test
{

    /**
     * 控制其行为的参数称为 policy 策略
     * 每一个 policy 都表现为键值对， 键与值都是编译期常量
     * 例： TAccuType 的取值合集为连加，连乘
     * DoAve 取值合集为true false
     * ValueType取float double 等
     */
    template <typename TAccuType, bool DoAve, typename ValueType>
    struct Accumulator
    {
    };

    /**
     * policy 对象， policy对象是编译期常量，包含键与值的全部信息，便于阅读
     * policy对象模板 构造与使用是分开的，首先要构造对象比如（PAve），并随后声明Accumulator的实例
     * 引入对象模板，元函数来构造policy对象，将policy对象构造的时机移到了使用之处，这样就无需准备大量的policy对象
     */

    // 定义policy和policy对象(template)
    /**
     * 将对象按照功能进行划分 每个模板可使用其中一个或多个policy对象
     * major class 和 minor class(次要类别) 如果均相同则互斥,不可同时引入
     * 用类型声明来表示键,值可以是类型，也可以是常量,数值或者模板
     */

    // 包含了Accumulator所需要的policy
    struct Accpolicy
    {
        // 定义所有的枚举类型,枚举集合
        struct AccuTypeCate
        {
            struct Add;
            struct Mul;
        };
        // 定义policy的键为Accu,默认值为Add
        using Accu = AccuTypeCate::Add;

        // 是否求平均值,默认为false, 是一个minor class
        struct IsAveValueCate;
        static constexpr bool IsAve = false;

        // 数据类型,默认为float, 是一个minor class
        struct ValueTypeCate;
        using Value = float;
    };

    // 策略容器模板类，用于存储和管理一系列策略
    template <typename... Tpolicies>
    struct PolicyContainer;

    // 类型特征变量，用于判断类型是否为PolicyContainer
    // 默认值为false
    template <typename T>
    constexpr bool isPolicyContainer = false;

    // PolicyContainer的特化版本，当类型匹配时返回true
    template <typename... Tpolicies>
    constexpr bool isPolicyContainer<PolicyContainer<Tpolicies...>> = true;

    // 子策略容器模板类，用于存储特定层级的策略
    template <typename TLayerNAme, typename... TPolicies>
    struct SubPolicyContainer;

    // 类型特征变量，用于判断类型是否为SubPolicyContainer
    // 默认值为false
    template <typename... T>
    constexpr bool isSubPolicyContainer = false;

    // SubPolicyContainer的特化版本，当类型匹配时返回true
    template <typename TLayer, typename... T>
    constexpr bool isSubPolicyContainer<SubPolicyContainer<TLayer, T...>> = true;

    // 策略选择结果模板类，用于存储策略选择后的结果
    template <typename TPolicyCont>
    struct PolicySelRes;

    // PolicySelRes的特化版本，当PolicyContainer只包含一个策略时
    // 直接继承该策略类型
    template <typename TPolicy>
    struct PolicySelRes<PolicyContainer<TPolicy>>
        : public TPolicy
    {
    };

    // PolicySelRes的特化版本，当PolicyContainer包含多个策略时
    // 继承除第一个策略外的剩余策略组成的PolicyContainer
    template <typename TCurPolicy, typename... TOtherPolicies>
    struct PolicySelRes<PolicyContainer<TCurPolicy, TOtherPolicies...>>
        : public PolicyContainer<TOtherPolicies...>
    {
        gbv
    };

    // 主过滤器模板类，用于过滤策略
    template <typename MCO, typename TMajorClass, typename... TP>
    struct MajorFilter_
    {
        using type = MCO; // 默认类型定义为MCO
    };

    // MajorFilter_的特化版本，用于处理策略过滤逻辑
    template <typename... TFilteredPolicies, typename TMajorClass,
              typename TCurPolicy, typename... TP>
    struct MajorFilter_<PolicyContainer<TFilteredPolicies...>, TMajorClass,
                        TCurPolicy, TP...>
    {
        // 内部实现模板类，用于策略过滤
        template <typename CurMajor, typename TDummy = void>
        struct _impl
        {
            // 如果当前策略的主类不匹配目标主类，则继续过滤剩余策略
            using type = typename MajorFilter_<PolicyContainer<TFilteredPolicies...>,
                                               TMajorClass, TP...>::type;
        };

        // 内部实现模板类的特化版本，当当前策略的主类匹配目标主类时
        template <typename TDummy>
        struct _impl<TMajorClass, TDummy>
        {
            // 如果匹配，则将当前策略添加到已过滤策略列表中
            using type = typename MajorFilter_<PolicyContainer<TFilteredPolicies..., TCurPolicy>,
                                               TMajorClass, TP...>::type;
        };

        // 使用内部实现类的类型定义
        using type = typename _impl<typename TCurPolicy::MajorClass>::type;
    };

} // namespace test