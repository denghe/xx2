#pragma once
#include "xx_mem.h"
#include "xx_typetraits.h"

namespace xx {

    // binary data referrer( buf + len ) / reader( buf + len + offset ) / container( buf + len + cap + offset ) C++20 std::span likely

    template<typename T> concept Has_GetBuf = requires(T t) { t.GetBuf(); };
    template<typename T> concept Has_GetLen = requires(T t) { t.GetLen(); };

    // referrer( buf + len )
    struct Span {
        uint8_t* buf;
        size_t len;

        Span()
            : buf(nullptr), len(0) {
        }
        Span(Span const& o) = default;
        Span& operator=(Span const& o) = default;

        [[maybe_unused]] Span(void const* buf, size_t len)
            : buf((uint8_t*)buf), len(len) {
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        [[maybe_unused]] explicit Span(T const& d)
            : buf((uint8_t*)d.buf), len(d.len) {
        }

        [[maybe_unused]] XX_INLINE void Reset(void const* buf_, size_t len_) {
            buf = (uint8_t*)buf_;
            len = len_;
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        [[maybe_unused]] XX_INLINE void Reset(T const& d, size_t offset_ = 0) {
            Reset(d.buf, d.len, offset_);
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        Span& operator=(T const& o) {
            Reset(o.buf, o.len);
            return *this;
        }

        XX_INLINE bool operator==(Span const& o) const {
            if (&o == this) return true;
            if (len != o.len) return false;
            return 0 == memcmp(buf, o.buf, len);
        }

        XX_INLINE bool operator!=(Span const& o) const {
            return !this->operator==(o);
        }

        XX_INLINE uint8_t& operator[](size_t idx) const {
            assert(idx < len);
            return (uint8_t&)buf[idx];
        }

        XX_INLINE operator bool() const {
            return len != 0;
        }

        // for easy use
        XX_INLINE operator std::string_view() const {
            return { (char*)buf, len };
        }
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

        Data_r()
                : offset(0) {
        }
        Data_r(Data_r const& o) = default;
        Data_r& operator=(Data_r const& o) = default;

        [[maybe_unused]] Data_r(void const* buf, size_t len, size_t offset = 0) {
            Reset(buf, len, offset);
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        [[maybe_unused]] Data_r(T const& d, size_t offset = 0) {
            if constexpr (std::is_same_v<std::string_view, std::decay_t<T>>) {
                Reset(d.data(), d.size(), offset);
            }
            else if constexpr (Has_GetBuf<T> && Has_GetLen<T>) {
                Reset(d.GetBuf(), d.GetLen(), offset);
            } else {
                Reset(d.buf, d.len, offset);
            }
        }

        [[maybe_unused]] XX_INLINE void Reset(void const* buf_, size_t len_, size_t offset_ = 0) {
            this->Span::Reset(buf_, len_);
            offset = offset_;
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        [[maybe_unused]] XX_INLINE void Reset(T const& d, size_t offset_ = 0) {
            Reset(d.buf, d.len, offset_);
        }

        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        Data_r& operator=(T const& o) {
            Reset(o.buf, o.len);
            return *this;
        }

        XX_INLINE bool operator==(Data_r const &o) {
            return this->Span::operator==(o);
        }

        XX_INLINE bool operator!=(Data_r const &o) {
            return !this->operator==(o);
        }

        XX_INLINE uint8_t* GetBuf() const {
            return buf;
        }

        XX_INLINE size_t GetLen() const {
            return len;
        }

        [[nodiscard]] bool HasLeft() const {
            return len > offset;
        }

        [[nodiscard]] size_t LeftLen() const {
            return len - offset;
        }

        [[nodiscard]] Span LeftSpan() const {
            return Span(buf + offset, len - offset);
        }

        [[nodiscard]] Data_r LeftData_r(size_t offset_ = 0) const {
            return Data_r(buf + offset, len - offset, offset_);
        }

        /***************************************************************************************************************************/
        // return !0 mean read fail.

        // return left buf + len( do not change offset )
        [[maybe_unused]] [[nodiscard]] XX_INLINE std::pair<uint8_t*, size_t> GetLeftBuf() {
            return { buf + offset, len - offset };
        }

        // dr.buf & len = left this.buf + len
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadLeftBuf(Data_r& dr) {
            if (offset == len) return __LINE__;
            dr.Reset(buf + offset, len - offset);
            offset = len;
            return 0;
        }

        // skip siz bytes
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadJump(size_t siz) {
            assert(siz);
            if (offset + siz > len) return __LINE__;
            offset += siz;
            return 0;
        }

        // memcpy fixed siz data( from offset ) to tar
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadBuf(void* tar, size_t siz) {
            assert(tar);
            if (offset + siz > len) return __LINE__;
            memcpy(tar, buf + offset, siz);
            offset += siz;
            return 0;
        }

        // memcpy fixed siz data( from specific idx ) to tar
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadBufAt(size_t idx, void* tar, size_t siz) const {
            assert(tar);
            if (idx + siz > len) return __LINE__;
            memcpy(tar, buf + idx, siz);
            return 0;
        }

        // return pointer( fixed len, from offset ) for memcpy
        [[maybe_unused]] [[nodiscard]] XX_INLINE void* ReadBuf(size_t siz) {
            if (offset + siz > len) return nullptr;
            auto bak = offset;
            offset += siz;
            return buf + bak;
        }

        // return pointer( fixed len, from specific idx ) for read / memcpy
        [[maybe_unused]] [[nodiscard]] XX_INLINE void* ReadBufAt(size_t idx, size_t siz) const {
            if (idx + siz > len) return nullptr;
            return buf + idx;
        }

        // read fixed len little endian number
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadFixed(T &v) {
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
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadFixedAt(size_t idx, T &v) {
            if (idx + sizeof(T) > len) return __LINE__;
            memcpy(&v, buf + idx, sizeof(T));
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            return 0;
        }

        // read fixed len big endian number
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadFixedBE(T& v) {
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
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadFixedBEAt(size_t idx, T& v) {
            if (idx + sizeof(T) >= len) return __LINE__;
            memcpy(&v, buf + idx, sizeof(T));
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            return 0;
        }

        // read fixed len little endian number array
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadFixedArray(T* tar, size_t siz) {
            assert(tar);
            if (offset + sizeof(T) * siz > len) return __LINE__;
            if constexpr (std::endian::native == std::endian::big) {
                auto p = buf + offset;
                T v;
                for (size_t i = 0; i < siz; ++i) {
                    memcpy(&v, p + i * sizeof(T), sizeof(T));
                    tar[i] = BSwap(v);
                }
            } else {
                memcpy(tar, buf + offset, sizeof(T) * siz);
            }
            offset += sizeof(T) * siz;
            return 0;
        }


        // read variable length integer
        template<typename T>
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadVarInteger(T &v) {
            using UT = std::make_unsigned_t<T>;
            UT u(0);
            for (size_t shift = 0; shift < sizeof(T) * 8; shift += 7) {
                if (offset == len) return __LINE__;
                auto b = (UT) buf[offset++];
                u |= UT((b & 0x7Fu) << shift);
                if ((b & 0x80) == 0) {
                    if constexpr (std::is_signed_v<T>) {
                        if constexpr (sizeof(T) <= 4) v = ZigZagDecode(uint32_t(u));
                        else v = ZigZagDecode(uint64_t(u));
                    } else {
                        v = u;
                    }
                    return 0;
                }
            }
            return __LINE__;
        }

        // read buf + len to sv
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadSV(std::string_view& sv, size_t siz) {
            if (offset + siz >= len) return __LINE__;
            auto s = (char*)buf + offset;
            offset += siz;
            sv = { s, siz };
            return 0;
        }

        // read "c string\0" to sv
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadCStr(std::string_view& sv) {
            return ReadSV(sv, strlen((char*)buf + offset));
        }
        [[maybe_unused]] [[nodiscard]] XX_INLINE int ReadCStr(std::string& s) {
            std::string_view sv;
            if (int r = ReadCStr(sv)) return r;
            s = sv;
            return 0;
        }

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


    /***************************************************************************************************************************/
    /***************************************************************************************************************************/


    // container( buf + len + cap + offset ). can reserve header len( for network package? )
    template<size_t bufHeaderReserveLen = 0>
    struct Data_rw : Data_r {
        size_t cap;

        Data_rw()
            : cap(0) {
        }

        // for easy use
        inline static size_t constexpr GetBufHeaderReserveLen() {
            return bufHeaderReserveLen;
        }

        // unsafe: override values
        [[maybe_unused]] XX_INLINE void Reset(void const* buf_ = nullptr, size_t len_ = 0, size_t offset_ = 0, size_t cap_ = 0) {
            this->Data_r::Reset(buf_, len_, offset_);
            cap = cap_;
        }

        // cap: reserve len
        [[maybe_unused]] explicit Data_rw(size_t cap)
                : cap(cap) {
            assert(cap);
            auto siz = Round2n(bufHeaderReserveLen + cap);
            buf = (new uint8_t[siz]) + bufHeaderReserveLen;
            this->cap = siz - bufHeaderReserveLen;
        }

        // memcpy( offset = 0 )
        [[maybe_unused]] Data_rw(Span const& s)
            : cap(0) {
            WriteBuf(s.buf, s.len);
        }

        // memcpy + set offset
        [[maybe_unused]] Data_rw(void const* ptr, size_t siz, size_t offset_ = 0)
            : cap(0) {
            WriteBuf(ptr, siz);
            offset = offset_;
        }

        // memcpy( offset = 0 )
        Data_rw(Data_rw const &o)
            : cap(0) {
            operator=(o);
        }

        // memcpy( offset = 0 )
        XX_INLINE Data_rw& operator=(Data_rw const& o) {
            return operator=<Data_rw>(o);
        }

        // memcpy( offset = 0 )( o have field: buf + len )
        template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
        XX_INLINE Data_rw& operator=(T const& o) {
            if (this == &o) return *this;
            Clear();
            WriteBuf(o.buf, o.len);
            return *this;
        }

        // move o's memory to this
        Data_rw(Data_rw &&o) noexcept {
            memcpy((void*)this, &o, sizeof(Data_rw));
            memset((void*)&o, 0, sizeof(Data_rw));
        }

        // swap
        XX_INLINE Data_rw &operator=(Data_rw &&o) noexcept {
            std::swap(buf, o.buf);
            std::swap(len, o.len);
            std::swap(cap, o.cap);
            std::swap(offset, o.offset);
            return *this;
        }

        // memcmp data( ignore offset, cap )
        XX_INLINE bool operator==(Data_rw const &o) const {
            return this->Span::operator==(o);
        }

        XX_INLINE bool operator!=(Data_rw const &o) const {
            return !this->operator==(o);
        }

        // ensure free space is enough( round2n == false usually for big file data, cap == len )
        template<bool checkCap = true, bool round2n = true>
        XX_NOINLINE void Reserve(size_t newCap) {
            if constexpr (checkCap) {
                if (newCap <= cap) return;
            }

            size_t siz;
            if constexpr (round2n) {
                siz = Round2n(bufHeaderReserveLen + newCap);
            } else {
                siz = bufHeaderReserveLen + newCap;
            }

            auto newBuf = new uint8_t[siz] + bufHeaderReserveLen;
            if (len) {
                memcpy(newBuf, buf, len);
            }

            // check cap for gcc issue
            if (cap) {
                delete[](buf - bufHeaderReserveLen);
            }
            buf = newBuf;
            cap = siz - bufHeaderReserveLen;
        }

        // buf cap resize to len
        void Shrink() {
            if (!len) {
                Clear(true);
            } else if (cap > len * 2) {
                auto newBuf = new uint8_t[bufHeaderReserveLen + len] + bufHeaderReserveLen;
                memcpy(newBuf, buf, len);
                delete[](buf - bufHeaderReserveLen);
                buf = newBuf;
                cap = len;
            }
        }

        // make a copy ( len == cap ) ( do not copy header )
        Data_rw ShrinkCopy() {
            Data_rw rtv;
            if (len) {
                rtv.buf = new uint8_t[bufHeaderReserveLen + len] + bufHeaderReserveLen;
                memcpy(rtv.buf, buf, len);
                rtv.cap = rtv.len = len;
            }
            return rtv;
        }

        // resize & return old len
        XX_INLINE size_t Resize(size_t newLen) {
            if (newLen > cap) {
                Reserve<false>(newLen);
            }
            auto rtv = len;
            len = newLen;
            return rtv;
        }

        // fill data by initializer list. will Clear. example: d.Fill({ 1,2,3. ....})
        template<typename T = int32_t, typename = std::enable_if_t<std::is_convertible_v<T, uint8_t>>>
        [[maybe_unused]] XX_INLINE void Fill(std::initializer_list<T> const &bytes) {
            Clear();
            Reserve(bytes.size());
            for (auto &&b : bytes) {
                buf[len++] = (uint8_t) b;
            }
        }

        // fill likely. make instance
        template<typename T = int32_t, typename = std::enable_if_t<std::is_convertible_v<T, uint8_t>>>
        static Data_rw From(std::initializer_list<T> const &bytes) {
            Data_rw d;
            d.Fill(bytes);
            return d;
        }

        template<typename T = int32_t, typename = std::enable_if_t<std::is_convertible_v<T, std::string_view>>>
        static Data_rw From(T const &sv) {
            Data_rw d;
            d.WriteBuf(sv);
            return d;
        }

        // erase data from head
        [[maybe_unused]] XX_INLINE void RemoveFront(size_t siz) {
            assert(siz <= len);
            if (!siz) return;
            len -= siz;
            if (len) {
                memmove(buf, buf + siz, len);
            }
        }

        // for fill read data
        Span GetFreeRange() const {
            return {buf + len, cap - len};
        }

        /***************************************************************************************************************************/

        // append data
        template<bool needReserve = true>
        XX_INLINE void WriteBuf(void const* ptr, size_t siz) {
            if constexpr (needReserve) {
                if (len + siz > cap) {
                    Reserve<false>(len + siz);
                }
            }
            memcpy(buf + len, ptr, siz);
            len += siz;
        }

        // support literal string[_view] for easy use
        template<bool needReserve = true>
        XX_INLINE void WriteBuf(std::string const& sv) {
            WriteBuf<needReserve>(sv.data(), sv.size());
        }
        template<bool needReserve = true>
        XX_INLINE void WriteBuf(std::string_view const& sv) {
            WriteBuf<needReserve>(sv.data(), sv.size());
        }
        template<bool needReserve = true, size_t n>
        XX_INLINE void WriteBuf(char const(&s)[n]) {
            WriteBuf<needReserve>(s, n - 1);
        }

        // write data to specific idx
        [[maybe_unused]] XX_INLINE void WriteBufAt(size_t idx, void const* ptr, size_t siz) {
            if (idx + siz > len) {
                Resize(idx + siz);
            }
            memcpy(buf + idx, ptr, siz);
        }

        // append write float / double / integer ( fixed size Little Endian )
        template<bool needReserve = true, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] XX_INLINE void WriteFixed(T v) {
            if constexpr (needReserve) {
                if (len + sizeof(T) > cap) {
                    Reserve<false>(len + sizeof(T));
                }
            }
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            memcpy(buf + len, &v, sizeof(T));
            len += sizeof(T);
        }

        // write float / double / integer ( fixed size Little Endian ) to specific index
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] XX_INLINE void WriteFixedAt(size_t idx, T v) {
            if (idx + sizeof(T) > len) {
                Resize(sizeof(T) + idx);
            }
            if constexpr (std::endian::native == std::endian::big) {
                v = BSwap(v);
            }
            memcpy(buf + idx, &v, sizeof(T));
        }

        // append write float / double / integer ( fixed size Big Endian )
        template<bool needReserve = true, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] XX_INLINE void WriteFixedBE(T v) {
            if constexpr (needReserve) {
                if (len + sizeof(T) > cap) {
                    Reserve<false>(len + sizeof(T));
                }
            }
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            memcpy(buf + len, &v, sizeof(T));
            len += sizeof(T);
        }

        // write float / double / integer ( fixed size Big Endian ) to specific index
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] XX_INLINE void WriteFixedBEAt(size_t idx, T v) {
            if (idx + sizeof(T) > len) {
                Resize(sizeof(T) + idx);
            }
            if constexpr (std::endian::native == std::endian::little) {
                v = BSwap(v);
            }
            memcpy(buf + idx, &v, sizeof(T));
        }

        // append write float / double / integer ( fixed size Little Endian ) array
        template<bool needReserve = true, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>>>
        [[maybe_unused]] XX_INLINE void WriteFixedArray(T const* ptr, size_t siz) {
            assert(ptr);
            if constexpr (needReserve) {
                if (len + sizeof(T) * siz > cap) {
                    Reserve<false>(len + sizeof(T) * siz);
                }
            }
            if constexpr (std::endian::native == std::endian::big) {
                auto p = buf + len;
                T v;
                for (size_t i = 0; i < siz; ++i) {
                    v = BSwap(ptr[i]);
                    memcpy(p + i * sizeof(T), &v, sizeof(T));
                }
            } else {
                memcpy(buf + len, ptr, sizeof(T) * siz);
            }
            len += sizeof(T) * siz;
        }

        // append write variable length integer( 7bit format )
        template<bool needReserve = true, typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        [[maybe_unused]] XX_INLINE void WriteVarInteger(T const &v) {
            using UT = std::make_unsigned_t<T>;
            UT u(v);
            if constexpr (std::is_signed_v<T>) {
                if constexpr (sizeof(T) <= 4) u = ZigZagEncode(int32_t(v));
                else u = ZigZagEncode(int64_t(v));
            }
            if constexpr (needReserve) {
                if (len + sizeof(T) + 2 > cap) {
                    Reserve<false>(len + sizeof(T) + 2);
                }
            }
            while (u >= 1 << 7) {
                buf[len++] = uint8_t((u & 0x7fu) | 0x80u);
                u = UT(u >> 7);
            }
            buf[len++] = uint8_t(u);
        }

        // skip specific size space. backup len and return
        template<bool needReserve = true>
        [[maybe_unused]] XX_INLINE size_t WriteJump(size_t siz) {
            auto bak = len;
            if constexpr (needReserve) {
                if (len + siz > cap) {
                    Reserve<false>(len + siz);
                }
            }
            len += siz;
            return bak;
        }

        // skip specific size space. backup pointer and return
        template<bool needReserve = true>
        [[maybe_unused]] XX_INLINE uint8_t* WriteSpace(size_t siz) {
            return buf + WriteJump<needReserve>(siz);
        }

        // multi write
        template<bool needReserve = true, typename ...TS>
        void Write(TS const& ...vs);

        // TS is base of Span. write buf only, do not write length
        template<bool needReserve = true, typename ...TS>
        void WriteBufSpans(TS const& ...vs) {
            (WriteBuf(vs.buf, vs.len), ...);
        }


        /***************************************************************************************************************************/

        ~Data_rw() {
            Clear(true);
        }

        // set len & offset = 0, support free buf( cap = 0 )
        XX_INLINE void Clear(bool freeBuf = false) {
            if (freeBuf && cap) {
                delete[](buf - bufHeaderReserveLen);
                buf = nullptr;
                cap = 0;
            }
            len = 0;
            offset = 0;
        }
    };

    // mem moveable tag
    template<size_t bufHeaderReserveLen>
    struct IsPod<Data_rw<bufHeaderReserveLen>, void> : std::true_type {};

    template<typename> struct IsData : std::false_type {};
    template<size_t S> struct IsData<Data_rw<S>> : std::true_type {};
    template<typename T> constexpr bool IsData_v = IsData<std::remove_cvref_t<T>>::value;
    //template<typename T> constexpr bool IsData_v = TemplateIsSame_v<std::remove_cvref_t<T>, Data_rw<1>>;

    using Data = Data_rw<sizeof(size_t)*2>;
    using DataView = Data_r;

    /************************************************************************************/
    // Data SerDe base template adapter

    template<typename T, typename ENABLED = void>
    struct DataFuncs {
        template<bool needReserve = true>
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

    template<size_t bufHeaderReserveLen>
    template<bool needReserve, typename ...TS>
    void Data_rw<bufHeaderReserveLen>::Write(TS const& ...vs) {
        (DataFuncs<TS>::template Write<needReserve>(*this, vs), ...);
    }

    /**********************************************************************************************************************/

    // adapt Data
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsData_v<T>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger<needReserve>(in.len);
            d.WriteBuf<needReserve>(in.buf, in.len);
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

    // adapt Span / Data_r ( for buf combine, does not write len )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_base_of_v<Span, T> && !IsData_v<T>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteBuf<needReserve>(in.buf, in.len);
        }
    };


    // adapt some number format for memcpy( 1 size or floating )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t< (std::is_arithmetic_v<T> && sizeof(T) == 1) || std::is_floating_point_v<T> >> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteFixed<needReserve>(in);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.ReadFixed(out);
        }
    };

    // adapt 2 ~ 8 size integer( variable length )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) >= 2>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger<needReserve>(in);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.ReadVarInteger(out);
        }
    };

    // adapt enum( forward to 1 ~ 8 size integer )
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_enum_v<T>>> {
        typedef std::underlying_type_t<T> U;
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.Write<needReserve>((U const&)in);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.Read((U&)out);
        }
    };

    // adapt std::string_view. write len + data
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_same_v<std::string_view, std::decay_t<T>>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger<needReserve>(in.size());
            d.WriteBuf<needReserve>((char*)in.data(), in.size());
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            DataFuncs<std::string_view, void>::Write<needReserve>(d, std::string_view(in));
        }
    };

    // adapt std::string. write len + data
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<std::is_same_v<std::string, std::decay_t<T>>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            DataFuncs<std::string_view, void>::Write<needReserve>(d, std::string_view(in));
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            if (in.has_value()) {
                d.Write<needReserve>((uint8_t)1, in.value());
            } else {
                d.Write<needReserve>((uint8_t)0);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.Write<needReserve>(in.first, in.second);
        }
        static inline int Read(Data_r& d, T& out) {
            return d.Read(out.first, out.second);
        }
    };

    // adapt std::tuple<......>
    template<typename T>
    struct DataFuncs<T, std::enable_if_t<IsStdTuple_v<T>>> {
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            std::apply([&](auto const &... args) {
                d.Write<needReserve>(args...);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            std::visit([&](auto&& v) {
                d.Write<needReserve>((size_t)in.index());
                d.Write<needReserve>(std::forward<decltype(v)>(v));
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            if constexpr (IsStdVector_v<T>) {
                d.WriteVarInteger<needReserve>(in.size());
                if (in.empty()) return;
            }
            if constexpr (sizeof(U) == 1 || std::is_floating_point_v<U>) {
                d.WriteFixedArray<needReserve>(in.data(), in.size());
            } else if constexpr (std::is_integral_v<U>) {
                if constexpr (needReserve) {
                    auto cap = in.size() * (sizeof(U) + 2);
                    if (d.cap < cap) {
                        d.Reserve<false>(cap);
                    }
                }
                for (auto&& o : in) {
                    d.WriteVarInteger<false>(o);
                }
            } else {
                for (auto&& o : in) {
                    d.Write<needReserve>(o);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger<needReserve>(in.size());
            if (in.empty()) return;
            if constexpr (std::is_integral_v<U>) {
                if constexpr (needReserve) {
                    auto cap = in.size() * (sizeof(U) + 2);
                    if (d.cap < cap) {
                        d.Reserve<false>(cap);
                    }
                }
                for (auto&& o : in) {
                    d.WriteVarInteger<false>(o);
                }
            } else {
                for (auto&& o : in) {
                    d.Write<needReserve>(o);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteVarInteger<needReserve>(in.size());
            for (auto&& kv : in) {
                d.Write<needReserve>(kv.first, kv.second);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            if (in) {
                d.Write<needReserve>((uint8_t)1, *in);
            } else {
                d.Write<needReserve>((uint8_t)0);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            assert(in.buf);
            assert(in.len);
            d.Write<needReserve>((U)*in.len);
            d.WriteFixedArray<needReserve>(in.buf, (U)*in.len);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteFixed<needReserve>((uint16_t)in.v);
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
        template<bool needReserve = true>
        static inline void Write(Data& d, T const& in) {
            d.WriteFixed<needReserve>((int16_t)in.v);
        }
        static inline int Read(Data_r& d, T& out) {
            int16_t tmp;
            auto r = d.ReadFixed(tmp);
            out.v = tmp;
            return r;
        }
    };

}
