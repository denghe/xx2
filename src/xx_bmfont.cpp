#include "xx_bmfont.h"
#include "xx_file.h"
#include "xx_gamebase.h"

namespace xx {
	
    // load binary .fnt & .pngs from file
    // return 0: success
    int32_t BMFont::Init(std::string_view fn) {
        auto p = GameBase::instance->GetFullPath(fn);
        auto d = ReadAllBytes_sv(p);
        if (d.len < 4) return __LINE__; // throw std::logic_error(ToString("BMFont file's size is too small. fn = ", p));
        if (std::string_view((char*)d.buf, 3) != "BMF"sv) return __LINE__; // throw std::logic_error(ToString("bad BMFont format. fn = ", p));
        if (d[3] != 3) return __LINE__; // throw std::logic_error(ToString("BMFont only support version 3. fn = ", p));
        if (auto r = Init(d.buf, d.len, p); r) return r;
        return 0;
    }

    // load font & texture from memory
    // tex: for easy load font texture from memory
    int32_t BMFont::Init(uint8_t const* buf_, size_t len_, std::string fullPath_, bool autoLoadTexture) {
        fullPath.clear();
        Data_r d{ buf_, len_ };

        // cleanup for logic safety
        memset(charArray.data(), 0, sizeof(charArray));
        charMap.clear();
        kernings.clear();
        if (autoLoadTexture) {
            texs.Clear();
        }
        paddingLeft = paddingTop = paddingRight = paddingBottom = fontSize = lineHeight = 0;

        List<std::string> texFNs;
        uint16_t pages{};

        (void)d.ReadJump(4);  // skip BMF\x3
        while (d.HasLeft()) {
            uint8_t blockId;
            if (auto r = d.ReadFixed(blockId)) return __LINE__; // throw std::logic_error(ToString("BMFont read blockId error. r = ", r, ". fn = ", p));
            uint32_t blockSize;
            if (auto r = d.ReadFixed(blockSize)) return __LINE__; // throw std::logic_error(ToString("BMFont read blockSize error. r = ", r, ". fn = ", p));
            if (d.offset + blockSize > d.len) return __LINE__; // throw std::logic_error(ToString("BMFont bad blockSize = ", blockSize, ". fn = ", p));

            Data_r dr(d.buf + d.offset, blockSize);
            if (blockId == 1) {
                /*
                    fontSize       2   int      0
                    bitField       1   bits     2  bit 0: smooth, bit 1: unicode, bit 2: italic, bit 3: bold, bit 4: fixedHeight, bits 5-7: reserved
                    charSet        1   uint     3
                    stretchH       2   uint     4
                    aa             1   uint     6
                    paddingUp      1   uint     7
                    paddingRight   1   uint     8
                    paddingDown    1   uint     9
                    paddingLeft    1   uint     10
                    spacingHoriz   1   uint     11
                    spacingVert    1   uint     12
                    outline        1   uint     13 added with version 2
                    fontName       n+1 string   14 null terminated string with length n
                    */

                if (auto r = dr.ReadFixed(fontSize)) return __LINE__; // throw std::logic_error(ToString("BMFont read fontSize error. r = ", r, ". fn = ", p));
                if (auto r = dr.ReadJump(5)) return __LINE__; // throw std::logic_error(ToString("BMFont read jump 5 error. r = ", r, ". fn = ", p));
                if (auto r = dr.ReadFixed(paddingTop)) return __LINE__; // throw std::logic_error(ToString("BMFont read paddingTop error. r = ", r, ". fn = ", p));
                if (auto r = dr.ReadFixed(paddingRight)) return __LINE__; // throw std::logic_error(ToString("BMFont read paddingRight error. r = ", r, ". fn = ", p));
                if (auto r = dr.ReadFixed(paddingBottom)) return __LINE__; // throw std::logic_error(ToString("BMFont read paddingBottom error. r = ", r, ". fn = ", p));
                if (auto r = dr.ReadFixed(paddingLeft)) return __LINE__; // throw std::logic_error(ToString("BMFont read paddingLeft error. r = ", r, ". fn = ", p));
                fontSize = std::abs(fontSize);  // maybe negative

            }
            else if (blockId == 2) {
                /*
                    lineHeight 2   uint    0
                    base       2   uint    2
                    scaleW     2   uint    4
                    scaleH     2   uint    6
                    pages      2   uint    8
                    bitField   1   bits    10  bits 0-6: reserved, bit 7: packed
                    alphaChnl  1   uint    11
                    redChnl    1   uint    12
                    greenChnl  1   uint    13
                    blueChnl   1   uint    14
                    */

                if (auto r = dr.ReadFixed(lineHeight)) return __LINE__; // throw std::logic_error(ToString("BMFont read lineHeight error. r = ", r, ". fn = ", p));
                if (auto r = dr.ReadJump(6)) return __LINE__; // throw std::logic_error(ToString("BMFont read jump 6 error. r = ", r, ". fn = ", p));

                if (auto r = dr.ReadFixed(pages)) return __LINE__; // throw std::logic_error(ToString("BMFont read pages error. r = ", r, ". fn = ", p));
                if (pages < 1) return __LINE__; // throw std::logic_error(ToString("BMFont pages < 1. fn = ", p));

            }
            else if (blockId == 3) {
                /*
                    pageNames 	p*(n+1) 	strings 	0 	p null terminated strings, each with length n
                    */

                for (int i = 0; i < (int)pages; ++i) {
                    auto&& texFN = texFNs.Emplace();
                    if (auto r = dr.ReadCStr(texFN)) return __LINE__; // throw std::logic_error(ToString("BMFont read pageNames[", i, "] error. r = ", r, ". fn = ", p));
                }

                // attach prefix dir name
                if (auto i = fullPath_.find_last_of("/"); i != fullPath_.npos) {
                    for (auto& s : texFNs) {
                        s = fullPath_.substr(0, i + 1) + s;
                    }
                }

            }
            else if (blockId == 4) {
                /*
                    id         4   uint    0+c*20  These fields are repeated until all characters have been described
                    x          2   uint    4+c*20
                    y          2   uint    6+c*20
                    width      2   uint    8+c*20
                    height     2   uint    10+c*20
                    xoffset    2   int     12+c*20
                    yoffset    2   int     14+c*20
                    xadvance   2   int     16+c*20
                    page       1   uint    18+c*20
                    chnl       1   uint    19+c*20
                    */

                for (uint32_t count = blockSize / 20, i = 0; i < count; i++) {
                    uint32_t id;
                    if (auto r = dr.ReadFixed(id)) return __LINE__; // throw std::logic_error(ToString("BMFont read id error. r = ", r, ". fn = ", p));

                    auto&& result = charMap.emplace(id, BMFontChar{});
                    if (!result.second) return __LINE__; // throw std::logic_error(ToString("BMFont insert to charRectMap error. BMFontChar id = ", id, ". fn = ", p));

                    auto& c = result.first->second;
                    if (auto r = dr.ReadFixed(c.x)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.x error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.y)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.y error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.width)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.width error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.height)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.height error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.xoffset)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.xoffset error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.yoffset)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.yoffset error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.xadvance)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.xadvance error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.page)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.page error. r = ", r, ". fn = ", p));
                    if (c.page >= pages) return __LINE__; // throw std::logic_error(ToString("BMFont c.page out of range. c.page = ", c.page, ", pages = ", pages, ". fn = ", p));
                    if (auto r = dr.ReadFixed(c.chnl)) return __LINE__; // throw std::logic_error(ToString("BMFont read c.chnl error. r = ", r, ". fn = ", p));

                    if (id < 256) {
                        charArray[id] = c;
                    }
                }
            }
            else if (blockId == 5) {
                /*
                    first  4   uint    0+c*10 	These fields are repeated until all kerning pairs have been described
                    second 4   uint    4+c*10
                    amount 2   int     8+c*10
                    */

                uint32_t first, second;
                int16_t amount;
                for (uint32_t count = blockSize / 10, i = 0; i < count; i++) {
                    if (auto r = dr.ReadFixed(first)) return __LINE__; // throw std::logic_error(ToString("BMFont read first error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(second)) return __LINE__; // throw std::logic_error(ToString("BMFont read second error. r = ", r, ". fn = ", p));
                    if (auto r = dr.ReadFixed(amount)) return __LINE__; // throw std::logic_error(ToString("BMFont read amount error. r = ", r, ". fn = ", p));

                    uint64_t key = ((uint64_t)first << 32) | ((uint64_t)second & 0xffffffffll);
                    kernings[key] = amount;
                }
            }

            if (auto r = d.ReadJump(blockSize)) return __LINE__; // throw std::logic_error(ToString("BMFont read jump blockSize error. blockSize = ", blockSize, ". r = ", r, ". fn = ", p));
        }

        // load textures
        if (autoLoadTexture) {
            for (auto&& f : texFNs) {
                texs.Emplace(GameBase::instance->LoadTexture(f));
            }
        }

        // fill texId
        for (auto& c : charArray) {
            c.texId = texs[c.page]->id;
        }
        for (auto& [k, v] : charMap) {
            v.texId = texs[v.page]->id;
        }

        // store display info when success
        fullPath = std::move(fullPath_);
        return 0;
    }

    // texture index: page
    BMFontChar const* BMFont::Get(char32_t charId) const {
        assert(charId >= 0);
        if (charId < 256) {
            auto& c = charArray[charId];
            if ((uint64_t&)c) {
                return &c;
            }
        }
        else {
            if (auto iter = charMap.find(charId); iter != charMap.end()) {
                return &iter->second;
            }
        }
        return nullptr;
    }

}
