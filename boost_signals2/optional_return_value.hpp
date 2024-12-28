
#pragma once

#include <optional>
#include <type_traits>

template<typename T>
struct return_value_holder_type
{
    using type = std::conditional<std::is_reference<T>::value,
        std::reference_wrapper<std::remove_cv_t<std::remove_reference_t<T>>>, T>::type;
};

