#pragma once
#include "xx_includes.h"

namespace xx {

    // binary data referrer( buf + len ) / reader( buf + len + offset ) / container( buf + len + cap + offset ) C++20 std::span likely

    template<typename T> concept Has_GetBuf = requires(T t) { t.GetBuf(); };
    template<typename T> concept Has_GetLen = requires(T t) { t.GetLen(); };

    // referrer( buf + len )
    struct Span {
        uint8_t* buf;
        size_t len;

        Span();
        Span(Span const& o) = default;
        Span& operator=(Span const& o) = default;
        Span(void const* buf, size_t len);

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        explicit Span(T const& d)
            : buf((uint8_t*)d.buf), len(d.len) {
        }

        void Reset(void const* buf_, size_t len_);

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        void Reset(T const& d, size_t offset_ = 0) {
            Reset(d.buf, d.len, offset_);
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        Span& operator=(T const& o) {
            Reset(o.buf, o.len);
            return *this;
        }

        bool operator==(Span const& o) const;
        bool operator!=(Span const& o) const;
        uint8_t& operator[](size_t idx) const;
        operator bool() const;
        operator std::string_view() const;  // for easy use
    };

    // mem moveable tag
    template<>
    struct IsPod<Span, void> : std::true_type {};

    // SerDe base template
    template<typename T, typename ENABLED>
    struct DataFuncs;

    // reader( buf + len + offset )
    struct Data_r : Span {
        size_t offset;

        Data_r();
        Data_r(Data_r const& o) = default;
        Data_r& operator=(Data_r const& o) = default;
        Data_r(void const* buf, size_t len, size_t offset = 0);

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        Data_r(T const& d, size_t offset = 0) {
            if constexpr (std::is_same_v<std::string_view, std::decay_t<T>>) {
                Reset(d.data(), d.size(), offset);
            }
            else if constexpr (Has_GetBuf<T> && Has_GetLen<T>) {
                Reset(d.GetBuf(), d.GetLen(), offset);
            }
            else {
                Reset(d.buf, d.len, offset);
            }
        }

        void Reset(void const* buf_, size_t len_, size_t offset_ = 0);

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        void Reset(T const& d, size_t offset_ = 0) {
            Reset(d.buf, d.len, offset_);
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        Data_r& operator=(T const& o) {
            Reset(o.buf, o.len);
            return *this;
        }

        bool operator==(Data_r const& o);
        bool operator!=(Data_r const& o);
        uint8_t* GetBuf() const;
        size_t GetLen() const;
        bool HasLeft() const;
        size_t LeftLen() const;
        Span LeftSpan() const;
        Data_r LeftData_r(size_t offset_ = 0) const;

        /***************************************************************************************************************************/
        // return !0 mean read fail.

        // return left buf + len( do not change offset )
        std::pair<uint8_t*, size_t> GetLeftBuf();

        // dr.buf & len = left this.buf + len
        int ReadLeftBuf(Data_r& dr);

        // skip siz bytes
        int ReadJump(size_t siz);

        // memcpy fixed siz data( from offset ) to tar
        int ReadBuf(void* tar, size_t siz);

        // memcpy fixed siz data( from specific idx ) to tar
        int ReadBufAt(size_t idx, void* tar, size_t siz) const;

        // return pointer( fixed len, from offset ) for memcpy
        void* ReadBuf(size_t siz);

        // return pointer( fixed len, from specific idx ) for read / memcpy
        void* ReadBufAt(size_t idx, size_t siz) const;

        // read fixed len little endian number
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        int ReadFixed(T& v) {
            if (offset + sizeof(T) > len) return __LINE__;
            memcpy(&v, buf + offset, sizeof(T));
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            offset += sizeof(T);
            return 0;
        }

        // read fixed len little endian number from specific idx ( do not change offset )
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        int ReadFixedAt(size_t idx, T& v) {
            if (idx + sizeof(T) > len) return __LINE__;
            memcpy(&v, buf + idx, sizeof(T));
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            return 0;
        }

        // read fixed len big endian number
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        int ReadFixedBE(T& v) {
            if (offset + sizeof(T) > len) return __LINE__;
            memcpy(&v, buf + offset, sizeof(T));
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            offset += sizeof(T);
            return 0;
        }

        // read fixed len big endian number from specific idx ( do not change offset )
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        int ReadFixedBEAt(size_t idx, T& v) {
            if (idx + sizeof(T) >= len) return __LINE__;
            memcpy(&v, buf + idx, sizeof(T));
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            return 0;
        }

        // read fixed len little endian number array
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        int ReadFixedArray(T* tar, size_t siz) {
            assert(tar);
            if (offset + sizeof(T) * siz > len) return __LINE__;
            if constexpr (std::endian::native == std::endian::big) {
                auto p = buf + offset;
                T v;
                for (size_t i = 0; i < siz; ++i) {
                    memcpy(&v, p + i * sizeof(T), sizeof(T));
                    tar[i] = BSwap(v);
                }
            }
            else {
                memcpy(tar, buf + offset, sizeof(T) * siz);
            }
            offset += sizeof(T) * siz;
            return 0;
        }


        // read variable length integer
        template<typename T>
        int ReadVarInteger(T& v) {
            using UT = std::make_unsigned_t<T>;
            UT u(0);
            for (size_t shift = 0; shift < sizeof(T) * 8; shift += 7) {
                if (offset == len) return __LINE__;
                auto b = (UT)buf[offset++];
                u |= UT((b & 0x7Fu) << shift);
                if ((b & 0x80) == 0) {
                    if constexpr (std::is_signed_v<T>) {
                        if constexpr (sizeof(T) <= 4) v = ZigZagDecode(uint32_t(u));
                        else v = ZigZagDecode(uint64_t(u));
                    }
                    else {
                        v = u;
                    }
                    return 0;
                }
            }
            return __LINE__;
        }

        // read buf + len to sv
        int ReadSV(std::string_view& sv, size_t siz);

        // read "c string\0" to sv
        int ReadCStr(std::string_view& sv);
        int ReadCStr(std::string& s);

        // multi read
        template<typename ...TS>
        int Read(TS&...vs) {
            return ReadCore(vs...);
        }

    protected:
        template<typename T, typename ...TS>
        int ReadCore(T& v, TS&...vs);
        template<typename T>
        int ReadCore(T& v);
    };

    // mem moveable tag
    template<>
    struct IsPod<Data_r, void> : std::true_type {};


    // container( buf + len + cap + offset )
    struct Data : Data_r {
        size_t cap;

        Data();

        // unsafe: override values
        void Reset(void const* buf_ = nullptr, size_t len_ = 0, size_t offset_ = 0, size_t cap_ = 0);

        // cap: reserve len
        explicit Data(size_t cap);

        // memcpy( offset = 0 )
        Data(Span const& s);

        // memcpy + set offset
        Data(void const* ptr, size_t siz, size_t offset_ = 0);

        // memcpy( offset = 0 )
        Data(Data const& o);

        // memcpy( offset = 0 )
        Data& operator=(Data const& o);

        // memcpy( offset = 0 )( o have field: buf + len )
        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        Data& operator=(T const& o) {
            if (this == &o) return *this;
            Clear();
            WriteBuf(o.buf, o.len);
            return *this;
        }

        // move o's memory to this
        Data(Data&& o) noexcept;

        // swap
        Data& operator=(Data&& o) noexcept;

        // memcmp data( ignore offset, cap )
        bool operator==(Data const& o) const;

        bool operator!=(Data const& o) const;

        // ensure free space is enough( round2n == false usually for big file data, cap == len )
        void Reserve(size_t newCap, bool round2n = true);

        // buf cap resize to len
        void Shrink();

        // make a copy ( len == cap ) ( do not copy header )
        Data ShrinkCopy();

        // resize & return old len
        size_t Resize(size_t newLen);

        // fill data by initializer list. will Clear. example: d.Fill({ 1,2,3. ....})
        template<typename T = int32_t, typename = std::enable_if_t<std::is_convertible_v<T, uint8_t>>>
        void Fill(std::initializer_list<T> const& bytes) {
            Clear();
            Reserve(bytes.size());
            for (auto&& b : bytes) {
                buf[len++] = (uint8_t)b;
            }
        }

        // fill likely. make instance
        template<typename T = int32_t, typename = std::enable_if_t<std::is_convertible_v<T, uint8_t>>>
        static Data From(std::initializer_list<T> const& bytes) {
            Data d;
            d.Fill(bytes);
            return d;
        }

        template<typename T = int32_t, typename = std::enable_if_t<std::is_convertible_v<T, std::string_view>>>
        static Data From(T const& sv) {
            Data d;
            d.WriteBuf(sv);
            return d;
        }

        // erase data from head
        void RemoveFront(size_t siz);

        // for fill read data
        Span GetFreeRange() const;

        // append data
        void WriteBuf(void const* ptr, size_t siz);
        // support literal string[_view] for easy use
        void WriteBuf(std::string const& sv);
        void WriteBuf(std::string_view const& sv);

        template<size_t n>
        void WriteBuf(char const(&s)[n]) {
            WriteBuf(s, n - 1);
        }

        // write data to specific idx
        void WriteBufAt(size_t idx, void const* ptr, size_t siz);

        // append write float / double / integer ( fixed size Little Endian )
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        void WriteFixed(T v) {
            if (len + sizeof(T) > cap) {
                Reserve(len + sizeof(T));
            }
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            memcpy(buf + len, &v, sizeof(T));
            len += sizeof(T);
        }

        // write float / double / integer ( fixed size Little Endian ) to specific index
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        void WriteFixedAt(size_t idx, T v) {
            if (idx + sizeof(T) > len) {
                Resize(sizeof(T) + idx);
            }
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            memcpy(buf + idx, &v, sizeof(T));
        }

        // append write float / double / integer ( fixed size Big Endian )
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        void WriteFixedBE(T v) {
            if (len + sizeof(T) > cap) {
                Reserve(len + sizeof(T));
            }
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            memcpy(buf + len, &v, sizeof(T));
            len += sizeof(T);
        }

        // write float / double / integer ( fixed size Big Endian ) to specific index
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        void WriteFixedBEAt(size_t idx, T v) {
            if (idx + sizeof(T) > len) {
                Resize(sizeof(T) + idx);
            }
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            memcpy(buf + idx, &v, sizeof(T));
        }

        // append write float / double / integer ( fixed size Little Endian ) array
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        void WriteFixedArray(T const* ptr, size_t siz) {
            assert(ptr);
            if (len + sizeof(T) * siz > cap) {
                Reserve(len + sizeof(T) * siz);
            }
            if constexpr (std::endian::native == std::endian::big) {
                auto p = buf + len;
                T v;
                for (size_t i = 0; i < siz; ++i) {
                    v = BSwap(ptr[i]);
                    memcpy(p + i * sizeof(T), &v, sizeof(T));
                }
            }
            else {
                memcpy(buf + len, ptr, sizeof(T) * siz);
            }
            len += sizeof(T) * siz;
        }

        // append write variable length integer( 7bit format )
        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        void WriteVarInteger(T const& v) {
            using UT = std::make_unsigned_t<T>;
            UT u(v);
            if constexpr (std::is_signed_v<T>) {
                if constexpr (sizeof(T) <= 4) u = ZigZagEncode(int32_t(v));
                else u = ZigZagEncode(int64_t(v));
            }
            if (len + sizeof(T) + 2 > cap) {
                Reserve(len + sizeof(T) + 2);
            }
            while (u >= 1 << 7) {
                buf[len++] = uint8_t((u & 0x7fu) | 0x80u);
                u = UT(u >> 7);
            }
            buf[len++] = uint8_t(u);
        }

        // skip specific size space. backup len and return
        size_t WriteJump(size_t siz);

        // skip specific size space. backup pointer and return
        uint8_t* WriteSpace(size_t siz);

        // multi write
        template<typename ...TS>
        void Write(TS const& ...vs);

        // TS is base of Span. write buf only, do not write length
        template<typename ...TS>
        void WriteBufSpans(TS const& ...vs) {
            (WriteBuf(vs.buf, vs.len), ...);
        }


        /***************************************************************************************************************************/

        ~Data();

        // set len & offset = 0, support free buf( cap = 0 )
        void Clear(bool freeBuf = false);
    };

    // mem moveable tag
    template<>
    struct IsPod<Data, void> : std::true_type {};

    template<typename> struct IsData : std::false_type {};
    template<> struct IsData<Data> : std::true_type {};
    template<typename T> constexpr bool IsData_v = IsData<std::remove_cvref_t<T>>::value;


    /************************************************************************************/
    // Data SerDe base template adapter

    template<typename T, typename ENABLED = void>
    struct DataFuncs {
        static inline void Write(Data& dw, T const& in) {
            auto tn = typeid(T).name();
            assert(false);
        }
        // return !0 mean error
        static inline int Read(Data_r& dr, T& out) {
            assert(false);
            return 0;
        }
    };

    /**********************************************************************************************************************/

    template<typename T, typename ...TS>
    int Data_r::ReadCore(T& v, TS&...vs) {
        if (auto r = DataFuncs<T>::Read(*this, v)) return r;
        return ReadCore(vs...);
    }
    template<typename T>
    int Data_r::ReadCore(T& v) {
        return DataFuncs<T>::Read(*this, v);
    }

    template<typename ...TS>
    void Data::Write(TS const& ...vs) {
        (DataFuncs<TS>::template Write(*this, vs), ...);
    }

    /**********************************************************************************************************************/

    // adapt Data
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsData_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger(in.len);
            d.WriteBuf(in.buf, in.len);
        }
        static inline int Read(Data_r& d, T& out) {
            size_t siz = 0;
            if (int r = d.ReadVarInteger(siz)) return r;
            if (d.offset + siz > d.len) return __LINE__;
            out.Clear();
            out.WriteBuf(d.buf + d.offset, siz);
            d.offset += siz;
            return 0;
        }
    };

    // adapt Span, Data_r ( for buf combine, does not write len )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_base_of_v<Span, T> && !IsData_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            d.WriteBuf(in.buf, in.len);
        }
    };


    // adapt some number format for memcpy( 1 size or floating )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< (std::is_arithmetic_v<T> && sizeof(T) == 1) || std::is_floating_point_v<T> >> {
        static inline void Write(Data& d, T const& in) {
            d.WriteFixed(in);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.ReadFixed(out);
        }
    };

    // adapt 2 ~ 8 size integer( variable length )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) >= 2>> {
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger(in);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.ReadVarInteger(out);
        }
    };

    // adapt enum( forward to 1 ~ 8 size integer )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_enum_v<T>>> {
        typedef std::underlying_type_t<T> U;
        static inline void Write(Data& d, T const& in) {
            d.Write((U const&)in);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.Read((U&)out);
        }
    };

    // adapt std::string_view. write len + data
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_same_v<std::string_view, std::decay_t<T>>>> {
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger(in.size());
            d.WriteBuf((char*)in.data(), in.size());
        }
        static inline int Read(Data_r& d, T& out) {
            size_t siz = 0;
            if (auto r = d.ReadVarInteger(siz)) return r;
            if (auto buf = d.ReadBuf(siz)) {
                out = std::string_view((char*)buf, siz);
                return 0;
            }
            return __LINE__;
        }
    };

    // adapt literal char[len]. write len(-1) + data( without end \0 )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsLiteral_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            DataFuncs<std::string_view, void>::Write(d, std::string_view(in));
        }
    };

    // adapt std::string. write len + data
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_same_v<std::string, std::decay_t<T>>>> {
        static inline void Write(Data& d, T const& in) {
            DataFuncs<std::string_view, void>::Write(d, std::string_view(in));
        }
        static inline int Read(Data_r& d, T& out) {
            size_t siz = 0;
            if (auto r = d.ReadVarInteger(siz)) return r;
            if (d.offset + siz > d.len) return __LINE__;
            out.assign((char*)d.buf + d.offset, siz);
            d.offset += siz;
            return 0;
        }
    };

    // adapt std::optional<T>
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsStdOptional_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            if (in.has_value()) {
                d.Write((uint8_t)1, in.value());
            } else {
                d.Write((uint8_t)0);
            }
        }
        static inline int Read(Data_r& d, T& out) {
            char hasValue = 0;
            if (int r = d.Read(hasValue)) return r;
            if (!hasValue) return 0;
            if (!out.has_value()) {
                out.emplace();
            }
            return d.Read(out.value());
        }
    };

    // adapt std::pair<K, V>
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsStdPair_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            d.Write(in.first, in.second);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.Read(out.first, out.second);
        }
    };

    // adapt std::tuple<......>
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsStdTuple_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            std::apply([&](auto const &... args) {
                d.Write(args...);
                }, in);
        }

        template<std::size_t I = 0, typename... Tp>
        static std::enable_if_t<I == sizeof...(Tp) - 1, int> ReadTuple(Data_r& d, std::tuple<Tp...>& t) {
            return d.Read(std::get<I>(t));
        }

        template<std::size_t I = 0, typename... Tp>
        static std::enable_if_t < I < sizeof...(Tp) - 1, int> ReadTuple(Data_r& d, std::tuple<Tp...>& t) {
            if (int r = d.Read(std::get<I>(t))) return r;
            return ReadTuple<I + 1, Tp...>(d, t);
        }

        static inline int Read(Data_r& d, T& out) {
            return ReadTuple(d, out);
        }
    };


    // adapt std::variant<......>
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsStdVariant_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            std::visit([&](auto&& v) {
                d.Write((size_t)in.index());
                d.Write(std::forward<decltype(v)>(v));
                }, in);
        }

        template<std::size_t I = 0, typename... Ts>
        static std::enable_if_t<I == sizeof...(Ts) - 1, int> ReadVariant(Data_r& d, std::variant<Ts...>& t, size_t const& index) {
            if (I == index) {
                t = std::tuple_element_t<I, std::tuple<Ts...>>();
                int r;
                std::visit([&](auto& v) {
                    r = d.Read(v);
                    }, t);
                return r;
            } else return -1;
        }
        template<std::size_t I = 0, typename... Ts>
        static std::enable_if_t < I < sizeof...(Ts) - 1, int> ReadVariant(Data_r& d, std::variant<Ts...>& t, size_t const& index) {
            if (I == index) {
                t = std::tuple_element_t<I, std::tuple<Ts...>>();
                int r;
                std::visit([&](auto& v) {
                    r = d.Read(v);
                    }, t);
                return r;
            } else return ReadVariant<I + 1, Ts...>(d, t, index);
        }

        static inline int Read(Data_r& d, T& out) {
            size_t index;
            if (int r = d.Read(index)) return r;
            return ReadVariant(d, out, index);
        }
    };


    // adapt std::vector, std::array   // todo: queue / deque
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< (IsStdVector_v<T> || IsStdArray_v<T>)>> {
        using U = typename T::value_type;
        static inline void Write(Data& d, T const& in) {
            if constexpr (IsStdVector_v<T>) {
                d.WriteVarInteger(in.size());
                if (in.empty()) return;
            }
            if constexpr (sizeof(U) == 1 || std::is_floating_point_v<U>) {
                d.WriteFixedArray(in.data(), in.size());
            } else if constexpr (std::is_integral_v<U>) {
                auto cap = in.size() * (sizeof(U) + 2);
                if (d.cap < cap) {
                    d.Reserve(cap);
                }
                for (auto&& o : in) {
                    d.WriteVarInteger<false>(o);
                }
            } else {
                for (auto&& o : in) {
                    d.Write(o);
                }
            }
        }
        static inline int Read(Data_r& d, T& out) {
            size_t siz = 0;
            if constexpr (IsStdVector_v<T>) {
                if (int r = d.ReadVarInteger(siz)) return r;
                if (d.offset + siz > d.len) return __LINE__;
                out.resize(siz);
                if (siz == 0) return 0;
            } else {
                siz = out.size();
            }
            auto buf = out.data();
            if constexpr (sizeof(U) == 1 || std::is_floating_point_v<U>) {
                if (int r = d.ReadFixedArray(buf, siz)) return r;
            } else {
                for (size_t i = 0; i < siz; ++i) {
                    if (int r = d.Read(buf[i])) return r;
                }
            }
            return 0;
        }
    };

    // adapt std::set, unordered_set
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< IsStdSetLike_v<T>>> {
        using U = typename T::value_type;
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger(in.size());
            if (in.empty()) return;
            if constexpr (std::is_integral_v<U>) {
                auto cap = in.size() * (sizeof(U) + 2);
                if (d.cap < cap) {
                    d.Reserve(cap);
                }
                for (auto&& o : in) {
                    d.WriteVarInteger<false>(o);
                }
            } else {
                for (auto&& o : in) {
                    d.Write(o);
                }
            }
        }
        static inline int Read(Data_r& d, T& out) {
            size_t siz = 0;
            if (int r = d.Read(siz)) return r;
            if (d.offset + siz > d.len) return __LINE__;
            out.clear();
            if (siz == 0) return 0;
            for (size_t i = 0; i < siz; ++i) {
                if (int r = Read_(d, out.emplace())) return r;
            }
            return 0;
        }
    };

    // adapt std::map unordered_map
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< IsStdMapLike_v<T>>> {
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger(in.size());
            for (auto&& kv : in) {
                d.Write(kv.first, kv.second);
            }
        }
        static inline int Read(Data_r& d, T& out) {
            size_t siz;
            if (int r = d.Read(siz)) return r;
            if (siz == 0) return 0;
            if (d.offset + siz * 2 > d.len) return __LINE__;
            for (size_t i = 0; i < siz; ++i) {
                std::pair<typename T::key_type, typename T::value_type> kv;
                if (int r = d.Read(kv.first, kv.second)) return r;
                out.insert(std::move(kv));
            }
            return 0;
        }
    };

    // adapt std::unique_ptr
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< IsStdUniquePtr_v<T> >> {
        static inline void Write(Data& d, T const& in) {
            if (in) {
                d.Write((uint8_t)1, *in);
            } else {
                d.Write((uint8_t)0);
            }
        }
        static inline int Read(Data_r& d, T& out) {
            uint8_t hasValue;
            if (int r = d.Read(hasValue)) return r;
            if (hasValue) {
                if (!out) {
                    out = std::make_unique<typename T::element_type>();
                }
                return d.Read(*out);
            } else {
                out.reset();
                return 0;
            }
        }
    };

    // for ref data( std::array<char, ?> buf; int len  likely )
    // example: xx::BufLenRef blr{ in.buf.data(), &in.len }; d.Read/Write( blr
    template<typename T, typename SizeType>
    struct BufLenRef {
        using ChildType = T;
        using S = SizeType;
        T* buf;
        S* len;
    };
    template<typename T> constexpr bool IsBufLenRef_v = TemplateIsSame_v<std::remove_cvref_t<T>, BufLenRef<AnyType, AnyType>>;

    // adapt BufLenRef
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< IsBufLenRef_v<T> >> {
        using U = std::make_unsigned_t<typename T::S>;
        static inline void Write(Data& d, T const& in) {
            assert(in.buf);
            assert(in.len);
            d.Write((U)*in.len);
            d.WriteFixedArray(in.buf, (U)*in.len);
        }
        static inline int Read(Data_r& d, T& out) {
            assert(out.buf);
            assert(out.len);
            if (auto r = d.Read(*(U*)out.len)) return r;
            return d.ReadFixedArray(out.buf, (U)*out.len);
        }
    };

    // float with uint16 value( for compress data )
    // example: d.Read( (xx::RWFloatUInt16&)x )
    struct RWFloatUInt16 {
        float v;
    };

    // adapt float with uint16 value
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< std::is_base_of_v<RWFloatUInt16, T> >> {
        static inline void Write(Data& d, T const& in) {
            d.WriteFixed((uint16_t)in.v);
        }
        static inline int Read(Data_r& d, T& out) {
            uint16_t tmp;
            auto r = d.ReadFixed(tmp);
            out.v = tmp;
            return r;
        }
    };

    // float with int16 value( for compress data )
    // example: d.Read( (xx::RWFloatInt16&)x )
    struct RWFloatInt16 {
        float v;
    };

    // adapt float with int16 value
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< std::is_base_of_v<RWFloatInt16, T> >> {
        static inline void Write(Data& d, T const& in) {
            d.WriteFixed((int16_t)in.v);
        }
        static inline int Read(Data_r& d, T& out) {
            int16_t tmp;
            auto r = d.ReadFixed(tmp);
            out.v = tmp;
            return r;
        }
    };

}
