#pragma once
#include "xx_gamebase_shader.h"
#include "xx_framebuffer.h"
#include "xx_frame.h"
#ifndef __EMSCRIPTEN__
#include "xx_bitmapdc.h"
// todo: mac support?
#endif

namespace xx {

    // todo: replace TinyFrame
    struct FontMaker {
        float charSize{}, padding{};
        XYu texSize{};
        XY canvasSize{};
        XY p;   // current tex's write pos offset
        std::string fontName;
        FrameBuffer fb;
        GLTexture canvasTex;  // single char texture
        std::unique_ptr<FontFrame[]> bases;
        std::unordered_map<char32_t, FontFrame> extras;
        xx::List<Ref<GLTexture>> texs;  // batchs

        // need ogl frame env
        void Init(float charSize_ = 24, float padding_ = 1, XYu texSize_ = { 2048, 2048 }, std::string fontName_ = "Arial") {
            charSize = charSize_;
            padding = padding_;
            texSize = texSize_;
            canvasSize = charSize_ / 0.75f;
            p = { 0, texSize_.y - 1 };
            fontName = std::move(fontName_);
            fb.Init();
            init_gCanvas(charSize, (int32_t)canvasSize.x, (int32_t)canvasSize.y, fontName.c_str());
            canvasTex = GLTexture::Create(canvasSize.As<uint32_t>());
            bases = std::make_unique<FontFrame[]>(256);
            texs.Emplace(GLTexture::CreateRef(texSize_));

            for (char32_t c = 0; c < 256; ++c) {
                MakeCharFrame(c);
            }
        }

        XX_INLINE FontFrame Find(char32_t c) {
            if (c < 256) {
                return bases[c];
            }
            else {
                if (auto iter = extras.find(c); iter != extras.end()) {
                    return iter->second;
                }
                else {
                    return MakeCharFrame(c);
                }
            }
        }

        auto operator[](char32_t c) {
            return Find(c);
        }


        // return total width
        template<typename S>
        float Measure(S const& s) {
            float w{};
            for (size_t i = 0; i < s.size(); ++i) {
                w += Find(s[i]).texRect.w;
            }
            return w;
        }

        FontFrame& MakeCharFrame(char32_t c) {
            char buf[16];
            buf[Char32ToUtf8(c, buf)] = '\0';

            // char: draw to canvas texture
            glBindTexture(GL_TEXTURE_2D, canvasTex.GetValue());
#ifndef __EMSCRIPTEN__
            auto [cw, ch] = upload_unicode_char_to_texture(charSize, buf);
#else
            float cw = upload_unicode_char_to_texture(charSize, buf);
            float ch = canvasSize.y;
#endif
            glBindTexture(GL_TEXTURE_2D, 0);

            FontFrame* f{};
            if (c < 256) {
                f = &bases[c];
            } else {
                auto rtv = extras.insert(std::make_pair(c, FontFrame{}));
                f = &rtv.first->second;
            }

            auto cp = p;
            if (p.x + cw > texSize.x) {                 // line wrap
                cp.x = 0;
                p.x = cw + padding;
                if (p.y - canvasSize.y < 0) {           // new page
                    texs.Emplace(GLTexture::CreateRef(texSize));
                    p.y = cp.y = texSize.y - 1;
                } else {                                // new line
                    p.y -= canvasSize.y;
                    cp.y = p.y;
                }
            } else {                                    // current line
                p.x += cw + padding;
            }

            auto& t = texs.Back();
            if (cw > 0) {
                UVRect rect{ 0, 0, (uint16_t)cw, (uint16_t)ch };
#ifndef __EMSCRIPTEN__
                auto pos = cp - texSize.As<float>() * 0.5f + XY{ 0, (canvasSize.y - ch) / 2 };
#else
                auto pos = cp - texSize.As<float>() * 0.5f;
#endif
                fb.DrawTo(t, {}, [&]() {
                    GameBase_shader::GetInstance()->Quad().Draw(this->canvasTex, rect, pos, { 0,1 });
                });
            }

            f->texId = t->GetValue();
            f->textureRect.x = uint16_t(cp.x);
            f->textureRect.y = uint16_t(texSize.y - 1 - cp.y);        // flip y for uv
            f->textureRect.w = uint16_t(cw);
            f->textureRect.h = uint16_t(ch);
            return *f;
        }

    };

}
