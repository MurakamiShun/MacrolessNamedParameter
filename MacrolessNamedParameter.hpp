/*
 * MacrolessNamedParameter: https://github.com/MurakamiShun/MacrolessNamedParameter
 * Copyright (c) 2022 MurakamiShun
 *
 * Released under the MIT License.
 */
#pragma once
#include <string_view>
#include <type_traits>

namespace MacrolessNamedParameter {
    namespace detail {
        template<typename CharT, std::size_t N>
        struct basic_arg_name {
            CharT data[N];
            consteval basic_arg_name(const CharT (&str)[N]) noexcept { for (std::size_t i = 0; i < N; ++i) data[i] = str[i]; }
            constexpr operator std::basic_string_view<CharT>() const {
                return std::basic_string_view<CharT>{ data, N - 1 };
            }
        };

        template<typename T, auto Name>
        struct arg_storage {
            using value_type = T;
            static constexpr auto name = Name;
            T value;
            arg_storage(T v) noexcept(noexcept(T(v))) : value(v){}
            template<typename Cast, std::enable_if_t<std::is_convertible_v<T, Cast>, std::nullptr_t> = nullptr>
            constexpr operator arg_storage<Cast, Name>()&& noexcept(noexcept(arg_storage<Cast, Name>{static_cast<Cast>(std::forward<T>(value))})){
                return arg_storage<Cast, Name>{static_cast<Cast>(std::forward<T>(value))};
            }
        };

        template<auto Name>
        struct arg_storage_builder {
            static constexpr auto name = Name;
            template<typename T>
            constexpr auto operator=(T&& value) const noexcept(noexcept(arg_storage<T, name>{std::forward<T>(value)})){
                return arg_storage<T, name>{std::forward<T>(value)};
            }
        };
    }

    template<detail::basic_arg_name Name, typename ArgType>
    using Arg = detail::arg_storage<ArgType, Name>;

    inline namespace literal {
        template<detail::basic_arg_name str>
        consteval auto operator"" _arg() noexcept {
            return detail::arg_storage_builder<str>{};
        }
    }
}
