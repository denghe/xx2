#pragma once
#include "xx_data.h"

namespace xx {
    // helpers

    /************************************************************************************/
    // StringFuncs base template for easy to append value to std::string
    /************************************************************************************/

    template<typename T, typename ENABLED = void>
    struct StringFuncs {
        static void Append(std::string& s, T const& in) {
            assert(false);
        }
        static void AppendCore(std::string& s, T const& in) {
            assert(false);
        }
    };

    /************************************************************************************/
    // Append / ToString
    /************************************************************************************/

    namespace Core {
        template<typename T>
        void Append(std::string &s, T const &v) {
            ::xx::StringFuncs<T>::Append(s, v);
        }
    }

    template<typename ...Args>
    void Append(std::string& s, Args const& ... args) {
        (::xx::Core::Append(s, args), ...);
    }

    template<std::size_t...Idxs, typename...TS>
    void AppendFormatCore(std::index_sequence<Idxs...>, std::string& s, size_t const& i, TS const&...vs) {
        (((i == Idxs) ? (Append(s, vs), 0) : 0), ...);
    }

    // append format, {0} {1}... ref to arguments
    template<typename...TS>
    size_t AppendFormat(std::string& s, char const* format, TS const&...vs) {
        std::array<std::pair<size_t, size_t>, sizeof...(vs)> cache{};
        size_t offset = 0;
        while (auto c = format[offset]) {
            if (c == '{') {
                c = format[++offset];
                if (c == '{') {
                    Append(s, '{');
                }
                else {
                    size_t i = 0;
                LabLoop:
                    c = format[offset];
                    if (c) {
                        if (c == '}') {
                            if (i >= sizeof...(vs)) return i;   // error
                            if (cache[i].second) {
                                s.append(s.data() + cache[i].first, cache[i].second);
                            }
                            else {
                                cache[i].first = s.size();
                                AppendFormatCore(std::make_index_sequence<sizeof...(TS)>(), s, i, vs...);
                                cache[i].second = s.size() - cache[i].first;
                            }
                            goto LabEnd;
                        }
                        else if (c < '0' || c > '9') {
                            return -1;  // error
                        } else {
                            i = i * 10 + (c - '0');
                        }
                        ++offset;
                        goto LabLoop;
                    }
                LabEnd:;
                }
            }
            else {
                Append(s, c);
            }
            ++offset;
        }
        return 0;
    }

    template<typename ...Args>
    std::string ToString(Args const& ... args) {
        std::string s;
        Append(s, args...);
        return s;
    }

    template<typename ...Args>
    std::string ToStringFormat(char const* format, Args const& ... args) {
        std::string s;
        AppendFormat(s, format, args...);
        return s;
    }

    // double to 1.234k  5M 7.8T  1e123 ...
    int ToStringEN(double d, char* o);

    // ucs4 to utf8. write to out. return len
    size_t Char32ToUtf8(char32_t c32, char* out);
    void StringU8ToU32(std::u32string& out, std::string_view const& sv);
    std::u32string StringU8ToU32(std::string_view const& sv);
    void StringU32ToU8(std::string& out, std::u32string_view const& sv);
    std::string StringU32ToU8(std::u32string_view const& sv);

    template<typename T>
    std::string const& U8AsString(T const& u8s) {
        return (std::string const&)u8s;
    }
    template<typename T>
    std::string U8AsString(T&& u8s) {
        return (std::string&&)u8s;
    }


    // example: __asdf__qwer_  ->  AsdfQwer
    std::string ToHump(std::string_view s, bool firstCharUpperCase = true);

    /************************************************************************************/
	// StringFuncs adapters for various types
    /************************************************************************************/

    // adapt char* \0
    template<>
    struct StringFuncs<char*, void> {
        static void Append(std::string& s, char* in) {
            s.append(in ? in: "null");
        }
    };

    // adapt char const* \0
    template<>
    struct StringFuncs<char const*, void> {
        static void Append(std::string& s, char const* in) {
            s.append(in ? in: "null");
        }
    };

    // adapt literal char[len] string
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsLiteral_v<T>>> {
        static void Append(std::string& s, T const& in) {
            s.append(in);
        }
    };

    // adapt std::string_view
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::string_view, T> || std::is_base_of_v<std::u8string_view, T>>> {
        static void Append(std::string& s, T const& in) {
            s.append((std::string_view const&)in);
        }
    };

    // adapt std::u32string_view
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::u32string_view, T>>> {
        static void Append(std::string& s, T const& in) {
            StringU32ToU8(s, in);
        }
    };

    // adapt std::string
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::string, T> || std::is_base_of_v<std::u8string, T>>> {
        static void Append(std::string& s, T const& in) {
            s.append((std::string const&)in);
        }
    };

    // adapt std::u32string
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::u32string, T>>> {
        static void Append(std::string& s, T const& in) {
            StringU32ToU8(s, in);
        }
    };

    // adapt std::filesystem::path
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::filesystem::path, T>>> {
        static void Append(std::string& s, T const& in) {
            auto u8s = in.u8string();
            s.append((std::string const&)u8s);
        }
    };

    // adapt type_info     typeid(T)
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<std::type_info, T>>> {
        static void Append(std::string& s, T const& in) {
            s.append(in.name());
        }
    };

	// for easy insert repeated char
    struct CharRepeater {
        char item;
        size_t len;
    };
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<CharRepeater, T>>> {
        static void Append(std::string& s, T const& in) {
            s.append(in.len, in.item);
        }
    };


    // adapt all numbers( char32_t -> utf8 )
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
        static void Append(std::string& s, T const& in) {
            if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
                s.append(in ? "true" : "false");
            }
            else if constexpr (std::is_same_v<char, std::decay_t<T>> || std::is_same_v<char8_t, std::decay_t<T>>) {
                s.push_back((char)in);
            }
            else if constexpr (std::is_floating_point_v<T>) {
                std::array<char, 40> buf;
                std::string_view sv;
#ifndef _MSC_VER
                if constexpr (sizeof(T) == 4) {
                    snprintf(buf.data(), buf.size(), "%.7f", in);
                } else {
                    static_assert(sizeof(T) == 8);
                    snprintf(buf.data(), buf.size(), "%.16lf", in);
                }
                sv = buf.data();
                if (sv.find('.') != sv.npos) {
                    if (auto siz = sv.find_last_not_of('0'); siz != sv.npos) {
                        if (sv[siz] == '.') --siz;
                        sv = std::string_view(sv.data(), siz + 1);
                    }
                }
#else
                auto [ptr, _] = std::to_chars(buf.data(), buf.data() + buf.size(), in, std::chars_format::general, sizeof(T) == 4 ? 7 : 16);
                sv = std::string_view(buf.data(), ptr - buf.data());
#endif
                s.append(sv);
            }
            else {
                if constexpr (std::is_same_v<char32_t, std::decay_t<T>>) {
                    char buf[8];
                    auto siz = Char32ToUtf8(in, buf);
                    s.append(std::string_view(buf, siz));
                }
                else {
                    //s.append(std::to_string(in));
                    std::array<char, 40> buf;
                    auto [ptr, _] = std::to_chars(buf.data(), buf.data() + buf.size(), in);
                    s.append(std::string_view(buf.data(), ptr - buf.data()));
                }
            }
        }
    };

    // adapt enum
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_enum_v<T>>> {
        static void Append(std::string& s, T const& in) {
            s.append(std::to_string((std::underlying_type_t<T>)in));
        }
    };

    // adapt TimePoint
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdTimepoint_v<T>>> {
        static void Append(std::string& s, T const& in) {
            AppendTimePoint_Local(s, in);
        }
    };

    // adapt std::optional
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdOptional_v<T>>> {
        static void Append(std::string &s, T const &in) {
            if (in.has_value()) {
                ::xx::Append(s, in.value());
            } else {
                s.append("null");
            }
        }
    };

    // adapt std::????set list std::vector std::array
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdSetLike_v<T> || IsStdArray_v<T> || IsStdList_v<T> || IsStdVector_v<T>>> {
        static void Append(std::string& s, T const& in) {
            s.push_back('[');
            if (!in.empty()) {
                for(auto&& o : in) {
                    ::xx::Append(s, o);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            }
            else {
                s.push_back(']');
            }
        }
    };

    // adapt std::????map
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdMapLike_v<T>>> {
        static void Append(std::string& s, T const& in) {
            s.push_back('[');
            if (!in.empty()) {
                for (auto &kv : in) {
                    ::xx::Append(s, kv.first);
                    s.push_back(',');
                    ::xx::Append(s, kv.second);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            }
            else {
                s.push_back(']');
            }
        }
    };

    // adapt std::pair
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdPair_v<T>>> {
        static void Append(std::string& s, T const& in) {
            s.push_back('[');
            ::xx::Append(s, in.first);
            s.push_back(',');
            ::xx::Append(s, in.second);
            s.push_back(']');
        }
    };
	
    // adapt std::tuple
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdTuple_v<T>>> {
        static void Append(std::string &s, T const &in) {
            s.push_back('[');
            std::apply([&](auto const &... args) {
                (::xx::Append(s, args, ','), ...);
                if constexpr(sizeof...(args) > 0) {
                    s.resize(s.size() - 1);
                }
            }, in);
            s.push_back(']');
        }
    };

    // adapt std::variant
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<IsStdVariant_v<T>>> {
        static void Append(std::string& s, T const& in) {
            std::visit([&](auto const& v) {
                ::xx::Append(s, v);
            }, in);
        }
    };

    // adapt Span, Data_r, Data_rw
    template<typename T>
    struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<Span, T>>> {
        static void Append(std::string& s, T const& in) {
            s.push_back('[');
            if (auto inLen = in.len) {
                for (size_t i = 0; i < inLen; ++i) {
                    ::xx::Append(s, (uint8_t)in[i]);
                    s.push_back(',');
                }
                s[s.size() - 1] = ']';
            }
            else {
                s.push_back(']');
            }
        }
    };


    /************************************************************************************/
    // utils
    /************************************************************************************/

    template<typename S>
    size_t StrLen(S const& s) {
        if constexpr (std::is_pointer_v<S>) {
            if (!s) return 0;
            using C = std::remove_cvref_t<std::remove_pointer_t<S>>;
            return std::basic_string_view<C>(s).size();
        }
        else  if constexpr (IsLiteral_v<S>) {
            return LiteralLen<S> - 1;
        }
        else {
            return s.size();
        }
    }

    template<typename S>
    auto StrPtr(S const& s) {
        return &s[0];
    }

    constexpr std::string_view base64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"sv;

    std::string Base64Encode(std::string_view const& in);

    std::string Base64Decode(std::string_view const& in);

    constexpr std::string_view intToStringChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"sv;

    // can't support negative interger now
    template<bool sClear = true, size_t fixedSize = 0, char fixedFill = '0', int toBase = 10, typename N, typename T>
    size_t IntToStringTo(T& s, N i) {
        constexpr int bufSiz{ sizeof(N) * 4 };
        std::array<char, bufSiz> buf;

        int idx{ bufSiz - 1 };
        while (i >= (N)toBase) {
            auto a = i / (N)toBase;
            auto b = i - a * (N)toBase;
            buf[idx--] = intToStringChars[b];
            i = a;
        }
        buf[idx] = intToStringChars[i];

        size_t numLen = bufSiz - idx;
        size_t len = numLen;
        if constexpr (fixedSize) {
            if (fixedSize > numLen) {
                len = fixedSize;
            }
        }

        if constexpr (sClear) {
            s.clear();
            s.reserve(len);
        } else {
            s.reserve(s.size() + len);
        }

        if constexpr (fixedSize) {
            if (fixedSize > numLen) {
                s.append(fixedSize - numLen, fixedFill);
            }
        }
        if constexpr (sizeof(typename T::value_type) == 1) {
            s.append((typename T::value_type*)&buf[0] + idx, numLen);
        } else {
            for (; idx < bufSiz; ++idx) {
                s.push_back((typename T::value_type)buf[idx]);
            }
        }

        return (size_t)len;
    }

    template<typename R = std::string, size_t fixedSize = 0, char fixedFill = '0', int toBase = 10, typename N>
    R IntToString(N i) {
        R s;
        ToStringTo<false, fixedSize, fixedFill, toBase>(s, i);
        return s;
    }


    struct ExpressionCalculator {
        int operator()(char const* exp_) {
            exp = exp_;
            return Expression();
        }
        template<typename S>
        int operator()(S const& exp_) {
            exp = exp_.data();
            return Expression();
        }
    protected:
        const char* exp{};
        char Peek() {
            return *exp;
        }
        char Get() {
            return *exp++;
        }
        int Number() {
            int result = Get() - '0';
            while (Peek() >= '0' && Peek() <= '9') {
                result = 10 * result + Get() - '0';
            }
            return result;
        }
        int Factor() {
            if (Peek() >= '0' && Peek() <= '9') return Number();
            else if (Peek() == '(') {
                Get(); // '('
                int result = Expression();
                Get(); // ')'
                return result;
            } else if (Peek() == '-') {
                Get();
                return -Factor();
            }
            return 0; // error
        }
        int Term() {
            int result = Factor();
            while (Peek() == '*' || Peek() == '/') {
                if (Get() == '*') {
                    result *= Factor();
                } else {
                    result /= Factor();
                }
            }
            return result;
        }
        int Expression() {
            int result = Term();
            while (Peek() == '+' || Peek() == '-') {
                if (Get() == '+') {
                    result += Term();
                } else {
                    result -= Term();
                }
            }
            return result;
        }
    };



    constexpr std::string_view TrimRight(std::string_view const& s) {
        auto idx = s.find_last_not_of(" \t\n\r\f\v");
        if (idx == std::string_view::npos) return { s.data(), 0 };
        return { s.data(), idx + 1 };
    }

    constexpr std::string_view TrimLeft(std::string_view const& s) {
        auto idx = s.find_first_not_of(" \t\n\r\f\v");
        if (idx == std::string_view::npos) return { s.data(), 0 };
        return { s.data() + idx, s.size() - idx };
    }

    constexpr std::string_view Trim(std::string_view const& s) {
        return TrimLeft(TrimRight(s));
    }

    template<size_t numDelimiters>
    constexpr std::string_view SplitOnce(std::string_view& sv, char const(&delimiters)[numDelimiters]) {
        static_assert(numDelimiters >= 2);
        auto data = sv.data();
        auto siz = sv.size();
        for (size_t i = 0; i != siz; ++i) {
            bool found;
            if constexpr (numDelimiters == 2) {
                found = sv[i] == delimiters[0];
            }
            else {
                found = std::string_view(delimiters).find(sv[i]) != std::string_view::npos;
            }
            if (found) {
                sv = std::string_view(data + i + 1, siz - i - 1);
                return {data, i};
            }
        }
        sv = std::string_view(data + siz, 0);
        return {data, siz};
    }

    template<typename T>
    constexpr bool SvToNumber(std::string_view const& input, T& out) {
        auto&& r = std::from_chars(input.data(), input.data() + input.size(), out);
        return r.ec != std::errc::invalid_argument && r.ec != std::errc::result_out_of_range;
    }

    template<typename T>
    constexpr T SvToNumber(std::string_view const& input, T const& defaultValue) {
        T out;
        auto&& r = std::from_chars(input.data(), input.data() + input.size(), out);
        return r.ec != std::errc::invalid_argument && r.ec != std::errc::result_out_of_range ? out : defaultValue;
    }

    template<typename T>
    constexpr std::optional<T> SvToNumber(std::string_view const& input) {
        T out;
        auto&& r = std::from_chars(input.data(), input.data() + input.size(), out);
        return r.ec != std::errc::invalid_argument && r.ec != std::errc::result_out_of_range ? out : std::optional<T>{};
    }

    template<typename T>
    std::string_view ToStringView(T const& v, char* buf, size_t len) {
        static_assert(std::is_integral_v<T>);
        if (auto [ptr, ec] = std::to_chars(buf, buf + len, v); ec == std::errc()) {
            return { buf, size_t(ptr - buf) };
        }
        else return {};
    }

    template<typename T, size_t len>
    std::string_view ToStringView(T const& v, char(&buf)[len]) {
        return ToStringView<T>(v, buf, len);
    }

    // convert s to T fill to dst
    template<typename T>
    void Convert(char const* s, T& dst) {
        if (!s) {
            dst = T();
        }
        else if constexpr (std::is_integral_v<T>&& std::is_unsigned_v<T> && sizeof(T) <= 4) {
            dst = (T)strtoul(s, nullptr, 0);
        }
        else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T> && sizeof(T) <= 4) {
            dst = (T)atoi(s);
        }
        else if constexpr (std::is_integral_v<T>&& std::is_unsigned_v<T> && sizeof(T) == 8) {
            dst = strtoull(s, nullptr, 0);
        }
        else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T> && sizeof(T) == 8) {
            dst = atoll(s);
        }
        else if constexpr (std::is_floating_point_v<T> && sizeof(T) == 4) {
            dst = strtof(s, nullptr);
        }
        else if constexpr (std::is_floating_point_v<T> && sizeof(T) == 8) {
            dst = atof(s);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            dst = s[0] == '1' || s[0] == 't' || s[0] == 'T' || s[0] == 'y' || s[0] == 'Y';
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            dst = s;
        }
        // todo: more
    }


    int FromHex(uint8_t c);
    uint8_t FromHex(char const* c);
    void ToHex(uint8_t c, uint8_t& h1, uint8_t& h2);
    void ToHex(std::string& s);

	// use s xor buf content. len align to 8 bytes
    void XorContent(uint64_t s, char* buf, size_t len);

    // use s xor b content
    void XorContent(char const* s, size_t slen, char* b, size_t blen);


    // remove full path's dir. remain file name
    int RemovePath(std::string& s);
	
	// scan level 1, 2 file extensions from file name
    std::pair<std::string_view, std::string_view> GetFileNameExts(std::string const& fn);

    // convert string all NUMBER CONTENT to fixed length & return ( for sort )
    std::string InnerNumberToFixed(std::string_view const& s, int n = 16);


    /************************************************************************************/
    // Cout
    /************************************************************************************/

    // replace std::cout
    template<typename...Args>
    void Cout(Args const& ...args) {
        std::string s;
        Append(s, args...);
        for (auto&& c : s) {
            if (!c) c = '^';
        }
        // std::cout << s;
        printf("%s", s.c_str());
    }

    // cout + content + endl
    template<typename...Args>
    void CoutN(Args const& ...args) {
        Cout(args...);
        //std::cout << std::endl;
        puts("");
    }

    // cout time + content + endl
    template<typename...Args>
    void CoutTN(Args const& ...args) {
        CoutN("[", std::chrono::system_clock::now(), "] ", args...);
    }

    // flush
    void CoutFlush();

    // cout + format string + args
    template<typename...Args>
    void CoutFormat(char const* const& format, Args const& ...args) {
        std::string s;
        AppendFormat(s, format, args...);
        for (auto&& c : s) {
            if (!c) c = '^';
        }
        //std::cout << s;
        printf("%s", s.c_str());
    }

    // cout + format string + args + endl
    template<typename...Args>
    void CoutNFormat(char const* const& format, Args const& ...args) {
        CoutFormat(format, args...);
        //std::cout << std::endl;
        puts("");
    }

    // cout + time + format string + args + endl
    template<typename...Args>
    void CoutTNFormat(char const* const& format, Args const& ...args) {
        CoutNFormat("[", std::chrono::system_clock::now(), "] ", args...);
    }
}
