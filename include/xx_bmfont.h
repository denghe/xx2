#pragma once
#include "xx_gl.h"
#include "xx_list.h"
#include "xx_ptr.h"

namespace xx {

    // reference from cocos2dx 3.x CCFontFNT.cpp  parseBinaryConfigFile
    // detail: http://www.angelcode.com/products/bmfont/doc/file_format.html
    // todo: kernings support

    struct BMFontChar {
        // char32_t id;
        uint16_t x, y, width, height;
        int16_t xoffset, yoffset, xadvance;
        uint8_t page, chnl;
        GLuint texId;
    };

    struct BMFont {
        std::array<BMFontChar, 256> charArray; // charMap ascii cache
        std::unordered_map<uint32_t, BMFontChar> charMap; // key: char id
        std::unordered_map<uint64_t, int> kernings;	// key: char id pair
        List<Shared<GLTexture>> texs;
        uint8_t paddingLeft{}, paddingTop{}, paddingRight{}, paddingBottom{};
        int16_t fontSize{};
        uint16_t lineHeight{};
        std::string fullPath;	// for display

        // load binary .fnt & .pngs from file
        // return 0: success
        int32_t Init(std::string_view fn);

        // load font & texture from memory
        // tex: for easy load font texture from memory
        int32_t Init(uint8_t const* buf_, size_t len_, std::string fullPath_, bool autoLoadTexture = true);

        // texture index: page
        BMFontChar const* Get(char32_t charId) const;
    };

}
