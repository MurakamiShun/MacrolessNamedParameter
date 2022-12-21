/*
 * MacrolessNamedParameter: https://github.com/MurakamiShun/MacrolessNamedParameter
 * Copyright (c) 2022 MurakamiShun
 *
 * Released under the MIT License.
 */
#pragma once
#include <array>
#include <string_view>
#include <type_traits>

namespace MacrolessNamedParameter{
    namespace detail{
        template<typename CharT, std::size_t N>
        struct basic_arg_name{
            const std::array<CharT, N> data;
            template<typename... T>
            consteval basic_arg_name(T... chars) : data{static_cast<CharT>(chars)...}{}
            constexpr operator std::basic_string_view<CharT>() const {
                return std::basic_string_view<CharT>{ data.data(), N-1 };
            }
        };

        template<typename... T>
        basic_arg_name(T... chars)->basic_arg_name<std::common_type_t<T...>, sizeof...(T)>;

        template<typename T, auto Name>
        struct arg_storage{
            using value_type = T;
            static constexpr auto name = Name;
            T value;
            template<typename Cast, std::enable_if_t<std::is_convertible_v<T, typename Cast::value_type>, std::nullptr_t> = nullptr>
            operator Cast() && {
                using cast_type = typename Cast::value_type;
                return arg_storage<cast_type, Name>{static_cast<cast_type>(value)};
            }
        };

        template<auto Name>
        struct arg_storage_builder{
            static constexpr auto name = Name;
            template<typename T>
            constexpr auto operator=(T&& value) const noexcept{
                return arg_storage<T, name>{std::forward<T>(value)};
            }
            template<typename T>
            using type = arg_storage<T, Name>;
        };
    }

    template<auto ArgStorageBuilder, typename ArgType>
    using Arg = detail::arg_storage<ArgType, ArgStorageBuilder.name>;

    inline namespace literal{
        template<typename T, T... chars>
        consteval auto operator"" _arg(){
            return detail::arg_storage_builder<detail::basic_arg_name<T, sizeof...(chars)+1>(chars..., '\0')>{};
        }
    }
}