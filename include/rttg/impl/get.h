
#pragma once

#include <functional>
#include <stdexcept>
#include <tuple>
#include <variant>

#include "variant_utils.h"

namespace rttg
{

namespace detail
{

template <std::size_t FirstIdx, std::size_t LastIdx, class TupleTRef>
auto binaryGetImpl(TupleTRef && tuple, std::size_t index) -> ToVariantRefT<std::remove_reference_t<TupleTRef>>
{
    using ReturnT = ToVariantRefT<std::remove_reference_t<TupleTRef>>;

    constexpr auto MiddleIdx = (FirstIdx + LastIdx) / 2U;
    if constexpr (FirstIdx == LastIdx)
    {
        return ReturnT{ std::in_place_index_t<MiddleIdx>{},
                        std::ref( std::get<MiddleIdx>( std::forward<TupleTRef>( tuple ) ) ) };
    }
    else
    {
        if constexpr (MiddleIdx > 0U)
        {
            if (MiddleIdx > index)
            {
                return binaryGetImpl<FirstIdx, MiddleIdx - 1>(std::forward<TupleTRef>(tuple), index);
            }
        }

        if (MiddleIdx < index)
        {
            return binaryGetImpl<MiddleIdx + 1U, LastIdx>(std::forward<TupleTRef>(tuple), index);
        }

        return ReturnT{ std::in_place_index_t<MiddleIdx>{},
                        std::ref( std::get<MiddleIdx>( std::forward<TupleTRef>( tuple ) ) ) };
    }
}

} // namespace detail


/**
 * Provides run-time access to @tuple element at @index.
 */
template <class TupleTRef>
auto get(TupleTRef && tuple, std::size_t index) -> ToVariantRefT<std::remove_reference_t<TupleTRef>>
{
    using TupleT = std::remove_reference_t<TupleTRef>;

    if (index >= std::tuple_size_v<TupleT>)
    {
        throw std::out_of_range("Error! Tuple index is out of range!\n");
    }

    return detail::binaryGetImpl<0U, std::tuple_size_v<TupleT> - 1U>(std::forward<TupleTRef>(tuple), index);
}

} // namespace rttg
