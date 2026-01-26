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

    void Frame::operator=(Shared<GLTexture> t) {
        tex = std::move(t);
        uvRect = tex->Rect();
        anchor = 0.5f;
    }

    void Frame::Clear() {
        tex.Reset();
    }

}
