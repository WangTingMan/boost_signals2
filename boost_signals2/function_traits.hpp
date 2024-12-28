#pragma once

#ifndef BOIST_SIGNALS2_FUNCTION_TRAITS_HPP
#define BOIST_SIGNALS2_FUNCTION_TRAITS_HPP

#include <cstdint>
#include <functional>
#include <tuple>

namespace boist {
namespace signals2 {

template<typename Function>
struct function_traits
{
    static inline constexpr bool is_function_pointer = false;
    static inline constexpr bool is_class_member = false;
    static inline constexpr bool is_normal_function = false;
};

template<typename R, typename ...Args>
struct function_traits<R( Args... )>
{
    using result_type = R;
    using function_type = R( Args... );

    static inline constexpr std::size_t arity = sizeof...( Args );
    static inline constexpr bool is_function_pointer = false;
    static inline constexpr bool is_class_member = false;
    static inline constexpr bool is_normal_function = true;

    template <std::size_t N>
    struct arg
    {
        static_assert( N < arity, "out of arity!" );
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
    using bare_tuple_type = std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>;
    using stl_function_type = std::function<R( Args... )>;
};

template<typename ReturnType, typename... Args>
struct function_traits<ReturnType( * )( Args... )> : function_traits<ReturnType( Args... )>
{
    static inline constexpr bool is_function_pointer = true;
};

template<typename ReturnType, typename... Args>
struct function_traits<std::function<ReturnType( Args... )>> : function_traits<ReturnType( Args... )>
{
    static inline constexpr bool is_normal_function = false;
};

template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType( ClassType::* )( Args... )> : function_traits<ReturnType( Args... )>
{
    static inline constexpr bool is_class_member = true;
    static inline constexpr bool is_normal_function = false;
    using class_type = ClassType;
};

template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType( ClassType::* )( Args... )const> : function_traits<ReturnType( ClassType::* )( Args... )>
{
    static inline constexpr bool is_class_member = true;
    static inline constexpr bool is_normal_function = false;
    using class_type = ClassType;
};

template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType( ClassType::* )( Args... )const volatile > : function_traits<ReturnType( ClassType::* )( Args... )>
{
    static inline constexpr bool is_class_member = true;
    static inline constexpr bool is_normal_function = false;
    using class_type = ClassType;
};

template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType( ClassType::* )( Args... )volatile> : function_traits<ReturnType( ClassType::* )( Args... )>
{
    static inline constexpr bool is_class_member = true;
    static inline constexpr bool is_normal_function = false;
    using class_type = ClassType;
};

}
}

#endif

