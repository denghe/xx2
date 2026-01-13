#pragma once
#include "xx_string.h"

namespace xx {

    int ToStringEN(double d, char* o) {
        if (d < 1) {
            o[0] = '0';
            return 1;
        }
        int len{};
        auto e = (int)std::log10(d);
        if (e < 3) {
            len = e + 1;
            auto n = (int)d;
            while (n >= 10) {
                auto a = n / 10;
                auto b = n - a * 10;
                o[e--] = (char)(b + 48);
                n = a;
            }
            o[0] = (char)(n + 48);
        } else {
            auto idx = e / 3;
            d /= std::pow(10, idx * 3);
            e = e - idx * 3;
            len = e + 1;
            auto n = (int)d;
            auto bak = n;
            while (n >= 10) {
                auto a = n / 10;
                auto b = n - a * 10;
                o[e--] = (char)(b + 48);
                n = a;
            }
            o[0] = (char)(n + 48);
            if (d > bak) {
                auto first = (int)((d - bak) * 10);
                if (first > 0) {
                    o[len++] = '.';
                    o[len++] = (char)(first + 48);
                }
            }
            if (idx < 10) {
                o[len++] = " KMGTPEZYB"[idx];
            } else {
                o[len++] = 'e';
                idx *= 3;
                len += (int)std::log10(idx) + 1;
                e = len - 1;
                n = idx;
                while (n >= 10) {
                    auto a = n / 10;
                    auto b = n - a * 10;
                    o[e--] = (char)(b + 48);
                    n = a;
                }
                o[e] = (char)(n + 48);
            }
        }
        return len;
    }

    size_t Char32ToUtf8(char32_t c32, char* out) {
        auto& c = (uint32_t&)c32;
        auto& o = (uint8_t*&)out;
        if (c < 0x7F) {
            o[0] = c;
            return 1;
        }
        else if (c < 0x7FF) {
            o[0] = 0b1100'0000 | (c >> 6);
            o[1] = 0b1000'0000 | (c & 0b0011'1111);
            return 2;
        }
        else if (c < 0x10000) {
            o[0] = 0b1110'0000 | (c >> 12);
            o[1] = 0b1000'0000 | ((c >> 6) & 0b0011'1111);
            o[2] = 0b1000'0000 | (c & 0b0011'1111);
            return 3;
        }
        else if (c < 0x110000) {
            o[0] = 0b1111'0000 | (c >> 18);
            o[1] = 0b1000'0000 | ((c >> 12) & 0b0011'1111);
            o[2] = 0b1000'0000 | ((c >> 6) & 0b0011'1111);
            o[3] = 0b1000'0000 | (c & 0b0011'1111);
            return 4;
        }
        else if (c < 0x1110000) {
            o[0] = 0b1111'1000 | (c >> 24);
            o[1] = 0b1000'0000 | ((c >> 18) & 0b0011'1111);
            o[2] = 0b1000'0000 | ((c >> 12) & 0b0011'1111);
            o[3] = 0b1000'0000 | ((c >> 6) & 0b0011'1111);
            o[4] = 0b1000'0000 | (c & 0b0011'1111);
            return 4;
        }
        assert(false);   // out of char32_t handled range
        return {};
    }

    void StringU8ToU32(std::u32string& out, std::string_view const& sv) {
        out.reserve(out.size() + sv.size());
        char32_t wc{};
        for (int i = 0; i < sv.size(); ) {
            char c = sv[i];
            if ((c & 0x80) == 0) {
                wc = c;
                ++i;
            } else if ((c & 0xE0) == 0xC0) {
                wc = (sv[i] & 0x1F) << 6;
                wc |= (sv[i + 1] & 0x3F);
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                wc = (sv[i] & 0xF) << 12;
                wc |= (sv[i + 1] & 0x3F) << 6;
                wc |= (sv[i + 2] & 0x3F);
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                wc = (sv[i] & 0x7) << 18;
                wc |= (sv[i + 1] & 0x3F) << 12;
                wc |= (sv[i + 2] & 0x3F) << 6;
                wc |= (sv[i + 3] & 0x3F);
                i += 4;
            } else if ((c & 0xFC) == 0xF8) {
                wc = (sv[i] & 0x3) << 24;
                wc |= (sv[i] & 0x3F) << 18;
                wc |= (sv[i] & 0x3F) << 12;
                wc |= (sv[i] & 0x3F) << 6;
                wc |= (sv[i] & 0x3F);
                i += 5;
            } else if ((c & 0xFE) == 0xFC) {
                wc = (sv[i] & 0x1) << 30;
                wc |= (sv[i] & 0x3F) << 24;
                wc |= (sv[i] & 0x3F) << 18;
                wc |= (sv[i] & 0x3F) << 12;
                wc |= (sv[i] & 0x3F) << 6;
                wc |= (sv[i] & 0x3F);
                i += 6;
            }
            out += wc;
        }
    }

    std::u32string StringU8ToU32(std::string_view const& sv) {
        std::u32string out;
        StringU8ToU32(out, sv);
        return out;
    }

    void StringU32ToU8(std::string& out, std::u32string_view const& sv) {
        for (int i = 0; i < sv.size(); ++i) {
            char32_t wc = sv[i];
            if (0 <= wc && wc <= 0x7f) {
                out += (char)wc;
            } else if (0x80 <= wc && wc <= 0x7ff) {
                out += (0xc0 | (wc >> 6));
                out += (0x80 | (wc & 0x3f));
            } else if (0x800 <= wc && wc <= 0xffff) {
                out += (0xe0 | (wc >> 12));
                out += (0x80 | ((wc >> 6) & 0x3f));
                out += (0x80 | (wc & 0x3f));
            } else if (0x10000 <= wc && wc <= 0x1fffff) {
                out += (0xf0 | (wc >> 18));
                out += (0x80 | ((wc >> 12) & 0x3f));
                out += (0x80 | ((wc >> 6) & 0x3f));
                out += (0x80 | (wc & 0x3f));
            } else if (0x200000 <= wc && wc <= 0x3ffffff) {
                out += (0xf8 | (wc >> 24));
                out += (0x80 | ((wc >> 18) & 0x3f));
                out += (0x80 | ((wc >> 12) & 0x3f));
                out += (0x80 | ((wc >> 6) & 0x3f));
                out += (0x80 | (wc & 0x3f));
            } else if (0x4000000 <= wc && wc <= 0x7fffffff) {
                out += (0xfc | (wc >> 30));
                out += (0x80 | ((wc >> 24) & 0x3f));
                out += (0x80 | ((wc >> 18) & 0x3f));
                out += (0x80 | ((wc >> 12) & 0x3f));
                out += (0x80 | ((wc >> 6) & 0x3f));
                out += (0x80 | (wc & 0x3f));
            }
        }
    }

    std::string StringU32ToU8(std::u32string_view const& sv) {
        std::string out;
        StringU32ToU8(out, sv);
        return out;
    }

    std::string ToHump(std::string_view s, bool firstCharUpperCase) {
        std::string r;
        if (!s.size()) return r;
        s = s.substr(s.find_first_not_of('_'));
        if (!s.size()) return r;

        auto e = s.size();
        r.reserve(e);
        if (firstCharUpperCase) {
            r.push_back(std::toupper(s[0]));
        } else {
            r.push_back(s[0]);
        }
        for (size_t i = 1; i < e; ++i) {
            if (s[i] != '_') {
                r.push_back(s[i]);
            } else {
                do {
                    ++i;
                    if (i >= e) return r;
                } while (s[i] == '_');
                r.push_back(std::toupper(s[i]));
            }
        }
        return r;
    }

    std::string Base64Encode(std::string_view const& in) {
        std::string out;
        int val = 0, valb = -6;
        for (uint8_t c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(base64chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) out.push_back(base64chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (out.size() % 4) out.push_back('=');
        return out;
    }

    std::string Base64Decode(std::string_view const& in) {
        std::string out;
        std::array<int, 256> T;
        T.fill(-1);
        for (int i = 0; i < 64; i++) T[base64chars[i]] = i;
        int val = 0, valb = -8;
        for (uint8_t c : in) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    int FromHex(uint8_t c) {
        if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
        else if (c >= 'a' && c <= 'z') return c - 'a' + 10;
        else if (c >= '0' && c <= '9') return c - '0';
        else return 0;
    }

    uint8_t FromHex(char const* c) {
        return ((uint8_t)FromHex(c[0]) << 4) | (uint8_t)FromHex(c[1]);
    }

    void ToHex(uint8_t c, uint8_t& h1, uint8_t& h2) {
        auto a = c / 16;
        auto b = c % 16;
        h1 = (uint8_t)(a + ((a <= 9) ? '0' : ('a' - 10)));
        h2 = (uint8_t)(b + ((b <= 9) ? '0' : ('a' - 10)));
    }

    void ToHex(std::string& s) {
        auto len = s.size();
        s.resize(len * 2);
        auto b = (uint8_t*)s.data();
        for (auto i = (ptrdiff_t)len - 1; i >= 0; --i) {
            ::xx::ToHex(b[i], b[i * 2], b[i * 2 + 1]);
        }
    }

    void XorContent(uint64_t s, char* buf, size_t len) {
        auto p = (char*)&s;
        auto left = len % sizeof(s);                                                                        // 余数 ( 这里假定 buf 一定会按 4/8 字节对齐, 小尾 )
        size_t i = 0;
        for (; i < len - left; i += sizeof(s)) {                                                            // 把字节对齐的部分肏了
            *(uint64_t*)&buf[i] ^= s;
        }
        for (auto j = i; i < len; ++i) {                                                                    // 余下部分单字节肏
            buf[i] ^= p[i - j];
        }
    }

    void XorContent(char const* s, size_t slen, char* b, size_t blen) {
        auto e = b + blen;
        for (size_t i = 0; b < e; *b++ ^= s[i++]) {
            if (i == slen) i = 0;
        }
    }

    int RemovePath(std::string& s) {
        auto b = s.data();
        auto e = (int)s.size() - 1;
        for (int i = e; i >= 0; --i) {
            if (b[i] == '/' || b[i] == '\\') {
                memmove(b, b + i + 1, e - i);
                s.resize(e - i);
                return i + 1;
            }
        }
        return 0;
    }
	
    std::pair<std::string_view, std::string_view> GetFileNameExts(std::string const& fn) {
        std::pair<std::string_view, std::string_view> rtv;
        auto dotPos = fn.rfind('.');
        auto extLen = fn.size() - dotPos;
        rtv.first = std::string_view(fn.data() + dotPos, extLen);
        if (dotPos) {
            dotPos = fn.rfind('.', dotPos - 1);
            if(dotPos != std::string::npos) {
                extLen = fn.size() - dotPos - extLen;
                rtv.second = std::string_view(fn.data() + dotPos, extLen);
            }
        }
        return rtv;
    }

    std::string InnerNumberToFixed(std::string_view const& s, int n) {
        std::string t, d;
        bool handleDigit = false;
        for (auto&& c : s) {
            if (c >= '0' && c <= '9') {
                if (!handleDigit) {
                    handleDigit = true;
                }
                d.append(1, c);
            }
            else {
                if (handleDigit) {
                    handleDigit = false;
                    t.append(n - d.size(), '0');
                    t.append(d);
                    d.clear();
                }
                else {
                    t.append(1, c);
                }
            }
        }
        if (handleDigit) {
            handleDigit = false;
            t.append(n - d.size(), '0');
            t.append(d);
            d.clear();
        }
        return t;
    }

    void CoutFlush() {
        //std::cout.flush();
        fflush(stdout);
    }

}
