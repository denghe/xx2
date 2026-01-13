#pragma once
#include "xx_list.h"

namespace xx {
	
    // reference from https://github.com/Reputeless/Xoshiro-cpp
    struct Rnd {
        std::array<uint32_t, 4> state;

        Rnd();
        Rnd(Rnd const&) = default;
        Rnd& operator=(Rnd const&) = default;

        void SetSeed(uint64_t seed);
        uint32_t Get();
        void NextBytes(void* buf, size_t len);
        std::string NextWord(size_t siz = 0, std::string_view chars = "abcdefghijklmnopqrstuvwxyz"sv);

        template<typename V = int32_t, class = std::enable_if_t<std::is_arithmetic_v<V>>>
        V Next() {
            if constexpr (std::is_same_v<bool, std::decay_t<V>>) {
                return Get() >= std::numeric_limits<uint32_t>::max() / 2;
            }
            else if constexpr (std::is_integral_v<V>) {
                std::make_unsigned_t<V> v;
                if constexpr (sizeof(V) <= 4) {
                    v = (V)Get();
                }
                else {
                    v = (V)(Get() | ((uint64_t)Get() << 32));
                }
                if constexpr (std::is_signed_v<V>) {
                    return (V)(v & std::numeric_limits<V>::max());
                }
                else return (V)v;
            }
            else if constexpr (std::is_floating_point_v<V>) {
                if constexpr (sizeof(V) == 4) {
                    return (float)(double(Get()) / 0xFFFFFFFFu);
                }
                else if constexpr (sizeof(V) == 8) {
                    constexpr auto max53 = (1ull << 53) - 1;
                    auto v = ((uint64_t)Get() << 32) | Get();
                    return double(v & max53) / max53;
                }
            }
            assert(false);
        }

        template<typename V>
        V Next(V from, V to) {
            if (from == to) return from;
            assert(from < to);
            if constexpr (std::is_floating_point_v<V>) {
                return from + Next<V>() * (to - from);
            }
            else {
                return from + Next<V>() % (to - from/* + 1*/);
            }
        }

        template<typename V>
        V Next2(V from, V to) {
            if (from == to) return from;
            if (to < from) {
                std::swap(from, to);
            }
            if constexpr (std::is_floating_point_v<V>) {
                return from + Next<V>() * (to - from);
            }
            else {
                return from + Next<V>() % (to - from/* + 1*/);
            }
        }

        template<typename V>
        V Next(V to) {
            return Next((V)0, to);
        }

        template<typename V>
        V Next(std::pair<V, V> const& fromTo) {
            return Next(fromTo.first, fromTo.second);
        }

        template<typename V>
        V NextRadians() {
            if constexpr (std::is_floating_point_v<V>) {
                return (V)Next<float>(-M_PI, M_PI);
            }
            else {
                static_assert((ptrdiff_t)sizeof(V) < 0, "unsupported type");
            }

        }

        template<typename T>
        auto& NextElement(T& container) {
            uint32_t idx;
            if constexpr (IsList_v<T>) {
                static_assert(sizeof(container.len) <= 4);
                idx = Next<uint32_t>(0, container.len);
            }
            else {
                assert(container.size() <= 0xFFFFFFFFu);
                idx = Next<uint32_t>(0, (uint32_t)container.size());
            }
            return container[idx];
        }
    };

}
