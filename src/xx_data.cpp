#include "xx_data.h"

namespace xx {

    Span::Span()
        : buf(nullptr), len(0) {
    }

    Span::Span(void const* buf, size_t len)
        : buf((uint8_t*)buf), len(len) {
    }

    void Span::Reset(void const* buf_, size_t len_) {
        buf = (uint8_t*)buf_;
        len = len_;
    }

    bool Span::operator==(Span const& o) const {
        if (&o == this) return true;
        if (len != o.len) return false;
        return 0 == memcmp(buf, o.buf, len);
    }

    bool Span::operator!=(Span const& o) const {
        return !this->operator==(o);
    }

    uint8_t& Span::operator[](size_t idx) const {
        assert(idx < len);
        return (uint8_t&)buf[idx];
    }

    Span::operator bool() const {
        return len != 0;
    }

    Span::operator std::string_view() const {
        return { (char*)buf, len };
    }








    Data_r::Data_r()
        : offset(0) {
    }

    Data_r::Data_r(void const* buf, size_t len, size_t offset) {
        Reset(buf, len, offset);
    }

    void Data_r::Reset(void const* buf_, size_t len_, size_t offset_) {
        this->Span::Reset(buf_, len_);
        offset = offset_;
    }

    bool Data_r::operator==(Data_r const& o) {
        return this->Span::operator==(o);
    }

    bool Data_r::operator!=(Data_r const& o) {
        return !this->operator==(o);
    }

    uint8_t* Data_r::GetBuf() const {
        return buf;
    }

    size_t Data_r::GetLen() const {
        return len;
    }

    bool Data_r::HasLeft() const {
        return len > offset;
    }

    size_t Data_r::LeftLen() const {
        return len - offset;
    }

    Span Data_r::LeftSpan() const {
        return Span(buf + offset, len - offset);
    }

    Data_r Data_r::LeftData_r(size_t offset_) const {
        return Data_r(buf + offset, len - offset, offset_);
    }

    std::pair<uint8_t*, size_t> Data_r::GetLeftBuf() {
        return { buf + offset, len - offset };
    }

    int Data_r::ReadLeftBuf(Data_r& dr) {
        if (offset == len) return __LINE__;
        dr.Reset(buf + offset, len - offset);
        offset = len;
        return 0;
    }

    int Data_r::ReadJump(size_t siz) {
        assert(siz);
        if (offset + siz > len) return __LINE__;
        offset += siz;
        return 0;
    }

    int Data_r::ReadBuf(void* tar, size_t siz) {
        assert(tar);
        if (offset + siz > len) return __LINE__;
        memcpy(tar, buf + offset, siz);
        offset += siz;
        return 0;
    }

    int Data_r::ReadBufAt(size_t idx, void* tar, size_t siz) const {
        assert(tar);
        if (idx + siz > len) return __LINE__;
        memcpy(tar, buf + idx, siz);
        return 0;
    }

    void* Data_r::ReadBuf(size_t siz) {
        if (offset + siz > len) return nullptr;
        auto bak = offset;
        offset += siz;
        return buf + bak;
    }

    void* Data_r::ReadBufAt(size_t idx, size_t siz) const {
        if (idx + siz > len) return nullptr;
        return buf + idx;
    }

    int Data_r::ReadSV(std::string_view& sv, size_t siz) {
        if (offset + siz >= len) return __LINE__;
        auto s = (char*)buf + offset;
        offset += siz;
        sv = { s, siz };
        return 0;
    }

    int Data_r::ReadCStr(std::string_view& sv) {
        return ReadSV(sv, strlen((char*)buf + offset));
    }
    int Data_r::ReadCStr(std::string& s) {
        std::string_view sv;
        if (int r = ReadCStr(sv)) return r;
        s = sv;
        return 0;
    }






    Data::Data()
        : cap(0) {
    }

    void Data::Reset(void const* buf_, size_t len_, size_t offset_, size_t cap_) {
        this->Data_r::Reset(buf_, len_, offset_);
        cap = cap_;
    }

    Data::Data(size_t cap)
        : cap(cap) {
        assert(cap);
        auto siz = Round2n(cap);
        buf = (new uint8_t[siz]);
        this->cap = siz;
    }

    Data::Data(Span const& s)
        : cap(0) {
        WriteBuf(s.buf, s.len);
    }

    Data::Data(void const* ptr, size_t siz, size_t offset_)
        : cap(0) {
        WriteBuf(ptr, siz);
        offset = offset_;
    }

    Data::Data(Data const& o)
        : cap(0) {
        operator=(o);
    }

    Data& Data::operator=(Data const& o) {
        return operator=<Data>(o);
    }

    Data::Data(Data&& o) noexcept {
        ::memcpy((void*)this, &o, sizeof(Data));
        ::memset((void*)&o, 0, sizeof(Data));
    }

    Data& Data::operator=(Data&& o) noexcept {
        std::swap(buf, o.buf);
        std::swap(len, o.len);
        std::swap(cap, o.cap);
        std::swap(offset, o.offset);
        return *this;
    }

    bool Data::operator==(Data const& o) const {
        return this->Span::operator==(o);
    }

    bool Data::operator!=(Data const& o) const {
        return !this->operator==(o);
    }

    XX_NOINLINE void Data::Reserve(size_t newCap, bool round2n) {
        if (newCap <= cap) return;

        size_t siz;
        if (round2n) {
            siz = Round2n(newCap);
        }
        else {
            siz = newCap;
        }

        auto newBuf = new uint8_t[siz];
        if (len) {
            ::memcpy(newBuf, buf, len);
        }

        // check cap for gcc issue
        if (cap) {
            delete[](buf);
        }
        buf = newBuf;
        cap = siz;
    }

    void Data::Shrink() {
        if (!len) {
            Clear(true);
        }
        else if (cap > len * 2) {
            auto newBuf = new uint8_t[len];
            ::memcpy(newBuf, buf, len);
            delete[](buf);
            buf = newBuf;
            cap = len;
        }
    }

    Data Data::ShrinkCopy() {
        Data rtv;
        if (len) {
            rtv.buf = new uint8_t[len];
            ::memcpy(rtv.buf, buf, len);
            rtv.cap = rtv.len = len;
        }
        return rtv;
    }

    size_t Data::Resize(size_t newLen) {
        if (newLen > cap) {
            Reserve(newLen);
        }
        auto rtv = len;
        len = newLen;
        return rtv;
    }

    void Data::RemoveFront(size_t siz) {
        assert(siz <= len);
        if (!siz) return;
        len -= siz;
        if (len) {
            memmove(buf, buf + siz, len);
        }
    }

    Span Data::GetFreeRange() const {
        return { buf + len, cap - len };
    }

    void Data::WriteBuf(void const* ptr, size_t siz) {
        if (len + siz > cap) {
            Reserve(len + siz);
        }
        memcpy(buf + len, ptr, siz);
        len += siz;
    }

    void Data::WriteBuf(std::string const& sv) {
        WriteBuf(sv.data(), sv.size());
    }

    void Data::WriteBuf(std::string_view const& sv) {
        WriteBuf(sv.data(), sv.size());
    }

    size_t Data::WriteJump(size_t siz) {
        auto bak = len;
        if (len + siz > cap) {
            Reserve(len + siz);
        }
        len += siz;
        return bak;
    }

    uint8_t* Data::WriteSpace(size_t siz) {
        return buf + WriteJump(siz);
    }

    void Data::WriteBufAt(size_t idx, void const* ptr, size_t siz) {
        if (idx + siz > len) {
            Resize(idx + siz);
        }
        memcpy(buf + idx, ptr, siz);
    }

    Data::~Data() {
        Clear(true);
    }

    void Data::Clear(bool freeBuf) {
        if (freeBuf && cap) {
            delete[](buf);
            buf = nullptr;
            cap = 0;
        }
        len = 0;
        offset = 0;
    }
}
