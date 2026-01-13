#pragma once

#include <chrono>
#include <array>
#include <type_traits>
#include <memory>
#include <utility>
#include <filesystem>
#include <functional>
#include <optional>
#include <variant>
#include <tuple>
#include <vector>
#include <queue>
#include <deque>
#include <string>
#include <initializer_list>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
//#include <sstream>
//#include <bit>
//#include <concepts>
//#include <charconv>
//#include <list>
//#include <stdexcept>
//#include <algorithm>
//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include <coroutine>
//#include <span>
//#include <format>


#define _USE_MATH_DEFINES  // needed for M_PI and M_PI2
#include <math.h>          // M_PI
#undef _USE_MATH_DEFINES
#include <cstring>
#include <cstdint>
#include <cassert>
#include <cstdlib>
//#include <cstddef>
//#include <ctime>
//#include <cstdio>
//#include <cerrno>

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace std::chrono_literals;


#ifndef XX_NOINLINE
#   ifndef NDEBUG
#       define XX_NOINLINE
#       define XX_INLINE inline
#   else
#       ifdef _MSC_VER
#           define XX_NOINLINE __declspec(noinline)
#           define XX_INLINE __forceinline
#       else
#           define XX_NOINLINE __attribute__((noinline))
#           define XX_INLINE __attribute__((always_inline)) inline
#       endif
#   endif
#endif


#ifndef XX_STRINGIFY
#	define XX_STRINGIFY(x)  XX_STRINGIFY_(x)
#	define XX_STRINGIFY_(x)  #x
#endif


#ifndef _countof
template<typename T, size_t N>
constexpr size_t _countof(T const (&arr)[N]) {
    return N;
}
#endif


#ifndef _offsetof
#define _offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif


#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - _offsetof(type, member)))
#endif


#ifndef _WIN32
inline void Sleep(int const& ms) {
    usleep(ms * 1000);
}
#endif


#ifdef __GNUC__
//#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif


/************************************************************************************/
// stackless simulate
/* example:

    int lineNumber{};   // struct member
    void Update() {
        XX_BEGIN(lineNumber)
            XX_YIELD(lineNumber)
        XX_END(lineNumber)
    }
	bool Update() {
		XX_BEGIN(lineNumber)
			XX_YIELD_F(lineNumber)
		XX_END(lineNumber)
	}

*/
#define XX_BEGIN(lineNumber)             switch (lineNumber) { case 0:
#define XX_YIELD(lineNumber)             lineNumber = __LINE__; return; case __LINE__:;
#define XX_YIELD_F(lineNumber)           lineNumber = __LINE__; return false; case __LINE__:;
#define XX_YIELD_B(lineNumber)           lineNumber = __LINE__; return false; case __LINE__:;
#define XX_YIELD_I(lineNumber)           lineNumber = __LINE__; return 0; case __LINE__:;
#define XX_YIELD_Z(lineNumber)           lineNumber = __LINE__; return 0; case __LINE__:;
#define XX_YIELD_TO_BEGIN(lineNumber)    lineNumber = 0; return;
#define XX_YIELD_F_TO_BEGIN(lineNumber)  lineNumber = 0; return false;
#define XX_YIELD_B_TO_BEGIN(lineNumber)  lineNumber = 0; return false;
#define XX_YIELD_I_TO_BEGIN(lineNumber)  lineNumber = 0; return 0;
#define XX_YIELD_Z_TO_BEGIN(lineNumber)  lineNumber = 0; return 0;
#define XX_END(lineNumber)               }



namespace xx {


    /************************************************************************************/
    // check T is baseof Template. usage: XX_IsTemplateOf(BT, T)::value

    struct IsTemplateOf {
        template <template <typename> typename TM, typename T>  static std::true_type  checkfunc(TM<T>);
        template <template <typename> typename TM>              static std::false_type checkfunc(...);
        template <template <int>   typename TM, int N>          static std::true_type  checkfunc(TM<N>);
        template <template <int>   typename TM>                 static std::false_type checkfunc(...);
    };
#define XX_IsTemplateOf(TM, ...) decltype(::xx::IsTemplateOf::checkfunc<TM>(std::declval<__VA_ARGS__>()))


    /************************************************************************************/
    // template is same checkers

    template< template<typename...>typename T> struct Template_t {};
    template<typename T> struct TemplateExt_t {
        static constexpr bool isTemplate{ false };
    };
    template< template<typename...> typename T, typename...Args> struct TemplateExt_t<T<Args...>> {
        static constexpr bool isTemplate{ true };
        using Type = Template_t<T>;
    };
    template<typename T, typename U> constexpr bool TemplateIsSame() {
        if constexpr (TemplateExt_t<T>::isTemplate != TemplateExt_t<U>::isTemplate) return false;
        else return std::is_same_v<typename TemplateExt_t<T>::Type, typename TemplateExt_t<U>::Type>;
    }
    template<typename T, typename U> constexpr bool TemplateIsSame_v = TemplateIsSame<T, U>();

    template <template <typename, auto> typename T> struct TemplateS_t {};
    template <template <typename, auto> typename T, typename E, auto S> struct TemplateExt_t<T<E, S>> {
        static constexpr bool isTemplate = true;
        using Type = TemplateS_t<T>;
    };

    struct AnyType {};  // helper type

    template<typename T> constexpr bool IsStdOptional_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::optional<AnyType>>;
    template<typename T> constexpr bool IsStdPair_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::pair<AnyType, AnyType>>;
    template<typename T> constexpr bool IsStdTuple_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::tuple<AnyType>>;
    template<typename T> constexpr bool IsStdVariant_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::variant<AnyType>>;
    template<typename T> constexpr bool IsStdUniquePtr_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::unique_ptr<AnyType>>;
    template<typename T> constexpr bool IsStdVector_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::vector<AnyType>>;
    template<typename T> constexpr bool IsStdList_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::list<AnyType>>;
    template<typename T> constexpr bool IsStdSet_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::set<AnyType>>;
    template<typename T> constexpr bool IsStdUnorderedSet_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::unordered_set<AnyType>>;
    template<typename T> constexpr bool IsStdMap_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::map<AnyType, AnyType>>;
    template<typename T> constexpr bool IsStdUnorderedMap_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::unordered_map<AnyType, AnyType>>;
    template<typename T> constexpr bool IsStdQueue_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::queue<AnyType>>;
    template<typename T> constexpr bool IsStdDeque_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::deque<AnyType>>;
    template<typename T> constexpr bool IsStdTimepoint_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::chrono::time_point<AnyType, AnyType>>;
    template<typename T> constexpr bool IsStdArray_v = TemplateIsSame_v<std::remove_cvref_t<T>, std::array<AnyType, 1>>;
    // ...

    template<typename T> constexpr bool IsStdSetLike_v = IsStdSet_v<T> || IsStdUnorderedSet_v<T>;
    template<typename T> constexpr bool IsStdMapLike_v = IsStdMap_v<T> || IsStdUnorderedMap_v<T>;
    template<typename T> constexpr bool IsStdQueueLike_v = IsStdQueue_v<T> || IsStdDeque_v<T>;
    // ...

    /************************************************************************************/
    // check T is literal "xxxxx" strings

    template<typename T, typename = void> struct IsLiteral : std::false_type {};
    template<size_t L> struct IsLiteral<char[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char);
    };
    template<size_t L> struct IsLiteral<char const [L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char);
    };
    template<size_t L> struct IsLiteral<char const (&)[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char);
    };
    template<size_t L> struct IsLiteral<char16_t[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char16_t);
    };
    template<size_t L> struct IsLiteral<char16_t const [L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char16_t);
    };
    template<size_t L> struct IsLiteral<char16_t const (&)[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char16_t);
    };
    template<size_t L> struct IsLiteral<char32_t[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char32_t);
    };
    template<size_t L> struct IsLiteral<char32_t const [L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char32_t);
    };
    template<size_t L> struct IsLiteral<char32_t const (&)[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(char32_t);
    };
    template<size_t L> struct IsLiteral<wchar_t[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(wchar_t);
    };
    template<size_t L> struct IsLiteral<wchar_t const [L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(wchar_t);
    };
    template<size_t L> struct IsLiteral<wchar_t const (&)[L], void> : std::true_type {
        static const size_t len = L;
        static const size_t wide = sizeof(wchar_t);
    };
    template<typename T> constexpr bool IsLiteral_v = IsLiteral<T>::value;
    template<typename T> constexpr size_t LiteralLen = IsLiteral<T>::len;
    template<typename T> constexpr size_t LiteralWide = IsLiteral<T>::wide;

    /************************************************************************************/
    // check T has member funcs: data() size()

    template<typename, typename = void> struct IsContainer : std::false_type {};
    template<typename T> struct IsContainer<T, std::void_t<decltype(std::declval<T>().data()), decltype(std::declval<T>().size())>> : std::true_type {};
    template<typename T>
    constexpr bool IsContainer_v = IsContainer<T>::value;

    /************************************************************************************/
    // check T has member func: operator()

    template<typename T, typename = void> struct IsCallable : std::is_function<T> {};
    template<typename T> struct IsCallable<T, typename std::enable_if<std::is_same<decltype(void(&T::operator())), void>::value>::type> : std::true_type {};
    template<typename T> constexpr bool IsCallable_v = IsCallable<T>::value;

    /************************************************************************************/
    // check T is std::function

    template<typename> struct IsFunction : std::false_type {};
    template<typename T> struct IsFunction<std::function<T>> : std::true_type {
        using FT = T;
    };
    template<typename T> constexpr bool IsFunction_v = IsFunction<std::remove_cvref_t<T>>::value;


    /************************************************************************************/
    // mapping operator(): return T, container, args, mutable

    template<typename T, typename = void>
    struct FuncTraits;

    template<typename Rtv, typename...Args>
    struct FuncTraits<Rtv(*)(Args ...)> {
        using R = Rtv;
        using A = std::tuple<Args...>;
        using A2 = std::tuple<std::decay_t<Args>...>;
        using C = void;
        static const bool isMutable = true;
    };

    template<typename Rtv, typename...Args>
    struct FuncTraits<Rtv(Args ...)> {
        using R = Rtv;
        using A = std::tuple<Args...>;
        using A2 = std::tuple<std::decay_t<Args>...>;
        using C = void;
        static const bool isMutable = true;
    };

    template<typename Rtv, typename CT, typename... Args>
    struct FuncTraits<Rtv(CT::*)(Args ...)> {
        using R = Rtv;
        using A = std::tuple<Args...>;
        using A2 = std::tuple<std::decay_t<Args>...>;
        using C = CT;
        static const bool isMutable = true;
    };

    template<typename Rtv, typename CT, typename... Args>
    struct FuncTraits<Rtv(CT::*)(Args ...) const> {
        using R = Rtv;
        using A = std::tuple<Args...>;
        using A2 = std::tuple<std::decay_t<Args>...>;
        using C = CT;
        static const bool isMutable = false;
    };

    template<typename T>
    struct FuncTraits<T, std::void_t<decltype(&T::operator())> >
        : public FuncTraits<decltype(&T::operator())> {
    };

    template<typename T> using FuncR_t = typename FuncTraits<T>::R;
    template<typename T> using FuncA_t = typename FuncTraits<T>::A;
    template<typename T> using FuncA2_t = typename FuncTraits<T>::A2;
    template<typename T> using FuncC_t = typename FuncTraits<T>::C;
    template<typename T> constexpr bool isMutable_v = FuncTraits<T>::isMutable;


    /************************************************************************************/
    // check tuple contains T

    template<typename T, typename Tuple> struct HasType;
    template<typename T> struct HasType<T, std::tuple<>> : std::false_type {};
    template<typename T, typename U, typename... Ts> struct HasType<T, std::tuple<U, Ts...>> : HasType<T, std::tuple<Ts...>> {};
    template<typename T, typename... Ts> struct HasType<T, std::tuple<T, Ts...>> : std::true_type {};
    template<typename T, typename Tuple> using TupleContainsType = typename HasType<T, Tuple>::type;
    template<typename T, typename Tuple> constexpr bool TupleContainsType_v = TupleContainsType<T, Tuple>::value;

    /************************************************************************************/
    // return T in tuple's index

    template<typename T, typename Tuple> struct TupleTypeIndex;
    template<typename T, typename...TS> struct TupleTypeIndex<T, std::tuple<T, TS...>> {
        static const size_t value = 0;
    };
    template<typename T, typename U, typename... TS> struct TupleTypeIndex<T, std::tuple<U, TS...>> {
        static const size_t value = 1 + TupleTypeIndex<T, std::tuple<TS...>>::value;
    };
    template<typename T, typename Tuple> constexpr size_t TupleTypeIndex_v = TupleTypeIndex<T, Tuple>::value;

    /************************************************************************************/
    // foreach tuple elements

    template <typename Tuple, typename F, std::size_t ...Indices>
    void ForEachCore(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
        using swallow = int[];
        (void)swallow {
            1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
        };
    }

    template <typename Tuple, typename F>
    void ForEach(Tuple&& tuple, F&& f) {
        constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
        ForEachCore(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<N>{});
    }

    /************************************************************************************/
    // foreach tuple types
    /*
        xx::ForEachType<Tup>([&]<typename T>() {
            // ...
        });
    */

    template<typename T, typename F>
    static inline constexpr void ForEachType(F&& func) {
        auto h = []<typename TT, typename FF, size_t... I>(FF && func, std::index_sequence<I...>) {
            (func.template operator() < std::tuple_element_t<I, TT> > (), ...);
        };
        h.template operator()<T> (
            std::forward<F>(func),
            std::make_index_sequence<std::tuple_size_v<T>>{}
        );
    }

    /************************************************************************************/
    // simple tuple memory like array<T, siz>

    template<typename ...TS> struct SimpleTuple;
    template<typename T> struct SimpleTuple<T> {
        T value;
    };
    template<typename T, typename ...TS> struct SimpleTuple<T, TS...> {
        T value;
        SimpleTuple<TS...> others;
    };

    template<typename T, typename...TS>
    auto&& Get(SimpleTuple<TS...>& t) {
        if constexpr (std::is_same_v< decltype(t.value), T>) {
            return t.value;
        }
        else {
            return Get<T>(t.others);
        }
    }

    /************************************************************************************/
    // ref args[index]

    template <int I, typename...Args>
    decltype(auto) GetAt(Args&&...args) {
        return std::get<I>(std::forward_as_tuple(std::forward<Args>(args)...));
    }


    /************************************************************************************/
    // std::????map<std::string, T>    .find( std::string_view
    // example:
    // std::unordered_map<std::string, XXXXXXXX, xx::StdStringHash, std::equal_to<>> m;

    struct StdStringHash {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char* txt) const {
            return std::hash<std::string_view>{}(txt);
        }
        [[nodiscard]] size_t operator()(std::string_view txt) const {
            return std::hash<std::string_view>{}(txt);
        }
        [[nodiscard]] size_t operator()(const std::string& txt) const {
            return std::hash<std::string>{}(txt);
        }
    };


    /***********************************************************************************/
    // replace std::aligned_storage & std::aligned_union

    template <typename T>
    class MyAlignedStorage {
    private:
        alignas(T) char dummyBuf[sizeof(T)];
    };

    template <typename... Ts>
    class MyAlignedUnion {
    private:
        alignas(MaxAlignof_v<Ts...>) char dummyBuf[MaxSizeof_v<Ts...>];
    };


    /************************************************************************************/
    // std::is_pod like, flag container can memcpy or memmove

    template<typename T, typename ENABLED = void>
    struct IsPod : std::false_type {};
    template<typename T>
    struct IsPod<T, std::enable_if_t<std::is_standard_layout_v<T>&& std::is_trivial_v<T>>> : std::true_type {};
    template<typename T> constexpr bool IsPod_v = IsPod<std::remove_cvref_t<T>>::value;


    /************************************************************************************/
    // scope guards

    template<class F>   // F == lambda
    auto MakeScopeGuard(F&& f) noexcept {
        struct ScopeGuard {
            F f;
            bool canceled;
            explicit ScopeGuard(F&& f) noexcept : f(std::forward<F>(f)), canceled(false) {}
            ~ScopeGuard() noexcept { if (!canceled) { f(); } }
            inline void Cancel() noexcept { canceled = true; }
            inline void operator()(bool canceled_ = false) {
                f();
                canceled = canceled_;
            }
        };
        return ScopeGuard(std::forward<F>(f));
    }

    template<class F>
    auto MakeSimpleScopeGuard(F&& f) noexcept {
        struct SG { F f; SG(F&& f) noexcept : f(std::forward<F>(f)) {} ~SG() { f(); } };
        return SG(std::forward<F>(f));
    }

    // return first bit '1' index
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    XX_INLINE size_t Calc2n(T n) {
        static_assert(sizeof(T) >= 4);
        return (sizeof(size_t) * 8 - 1) - std::countl_zero(n);
    }

    // return 2^x ( >= n )
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    XX_INLINE T Round2n(T n) {
        static_assert(sizeof(T) >= 4);
        auto shift = Calc2n(n);
        auto rtv = T(1) << shift;
        if (rtv == n) return n;
        else return rtv << 1;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    XX_INLINE bool IsPowerOfTwo(T n) {
        return (n != 0) && ((n & (n - 1)) == 0);
    }

    template<typename T, typename ...Args>
    XX_INLINE T& ReNew(T& o, Args&& ...args) {
        std::destroy_at(&o);
        return *std::construct_at(&o, std::forward<Args>(args)...);
    }

    template<typename T>
    XX_INLINE T BSwap(T i) {
        T r;
#ifdef _WIN32
        if constexpr (sizeof(T) == 2) *(uint16_t*)&r = _byteswap_ushort(*(uint16_t*)&i);
        if constexpr (sizeof(T) == 4) *(uint32_t*)&r = _byteswap_ulong(*(uint32_t*)&i);
        if constexpr (sizeof(T) == 8) *(uint64_t*)&r = _byteswap_uint64(*(uint64_t*)&i);
#else
        if constexpr (sizeof(T) == 2) *(uint16_t*)&r = __builtin_bswap16(*(uint16_t*)&i);
        if constexpr (sizeof(T) == 4) *(uint32_t*)&r = __builtin_bswap32(*(uint32_t*)&i);
        if constexpr (sizeof(T) == 8) *(uint64_t*)&r = __builtin_bswap64(*(uint64_t*)&i);
#endif
        return r;
    }

    // signed int decode: return (in is singular: negative) ? -(in + 1) / 2 : in / 2
    XX_INLINE int16_t ZigZagDecode(uint16_t in) {
        return (int16_t)((int16_t)(in >> 1) ^ (-(int16_t)(in & 1)));
    }
    XX_INLINE int32_t ZigZagDecode(uint32_t in) {
        return (int32_t)(in >> 1) ^ (-(int32_t)(in & 1));
    }
    XX_INLINE int64_t ZigZagDecode(uint64_t in) {
        return (int64_t)(in >> 1) ^ (-(int64_t)(in & 1));
    }

    // signed int encode: return in < 0 ? (-in * 2 - 1) : (in * 2)
    XX_INLINE uint16_t ZigZagEncode(int16_t in) {
        return (uint16_t)((in << 1) ^ (in >> 15));
    }
    XX_INLINE uint32_t ZigZagEncode(int32_t in) {
        return (in << 1) ^ (in >> 31);
    }
    XX_INLINE uint64_t ZigZagEncode(int64_t in) {
        return (in << 1) ^ (in >> 63);
    }

    // flag enum bit check
    template<typename T, class = std::enable_if_t<std::is_enum_v<T>>>
    XX_INLINE bool FlagContains(T const& a, T const& b) {
        using U = std::underlying_type_t<T>;
        return ((U)a & (U)b) != U{};
    }

    // for some search result
    enum class ForeachResult {
        Continue,
        RemoveAndContinue,
        Break,
        RemoveAndBreak
    };

}
