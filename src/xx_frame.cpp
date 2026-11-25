#include "xx_frame.h"

namespace xx {
	
    TinyFrame::operator GLuint () const {
        return tex->id;
    }

    TinyFrame::operator UVRect () const {
        return uvRect;
    }

    TinyFrame::operator bool() const {
        return tex.pointer != nullptr;
    }

    XY TinyFrame::Size() const {
        return { uvRect.w, uvRect.h };
    }

    TinyFrame::TinyFrame(Shared<GLTexture> t) {
        operator=(std::move(t));
    }

    TinyFrame::TinyFrame(Shared<GLTexture> t, int32_t x, int32_t y, int32_t w, int32_t h) {
        tex = std::move(t);
        uvRect = { (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h };
    }

    void TinyFrame::operator=(Shared<GLTexture> t) {
        tex = std::move(t);
        uvRect = tex->Rect();
    }

    void TinyFrame::Clear() {
        tex.Reset();
    }


    Frame::operator TinyFrame& () const {
        return *(TinyFrame*)this;
    }
    Frame::operator TinyFrame* () const {
        return (TinyFrame*)this;
    }

    Frame::operator GLuint () const {
        return tex->id;
    }

    Frame::operator UVRect () const {
        return uvRect;
    }

    Frame::operator XY const& () const {
        return anchor;
    }

    Frame::operator bool() const {
		return tex.pointer != nullptr;
    }

    XY Frame::Size() const {
        return { uvRect.w, uvRect.h };
    }

    Frame::Frame(Shared<GLTexture> t) {
        operator=(std::move(t));
    }

    Frame::Frame(Shared<GLTexture> t, int32_t x, int32_t y, int32_t w, int32_t h, XY a, int32_t e) {
        tex = std::move(t);
        uvRect = { (uint16_t)x, (uint16_t)y, (uint16_t)w, (uint16_t)h };
        anchor = a;
        extrude = e;
    }

    void Frame::operator=(Shared<GLTexture> t) {
        tex = std::move(t);
        uvRect = tex->Rect();
        uvTrim = {};
        anchor = 0.5f;
        extrude = 0;
    }

    XY Frame::CalcTrimAnchor(XY a) const {
        auto oy = /* uvTrim.y */ uvTrim.h - uvTrim.y - uvRect.h;  // for opengl flip Y
        return { (a.x * uvTrim.w - uvTrim.x) / uvRect.w, (a.y * uvTrim.h - oy) / uvRect.h };
    }

    void Frame::Clear() {
        tex.Reset();
    }

}
