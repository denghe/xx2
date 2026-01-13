#pragma once
#include <gl.h>
#include "xx_data.h"
#include "xx_prims.h"

namespace xx {

#ifndef NDEBUG
    void CheckGLErrorAt(const char* file, int line, const char* func);
#define CheckGLError() ::xx::CheckGLErrorAt(__FILE__, __LINE__, __FUNCTION__)
#else
#define CheckGLError() ((void)0)
#endif

    enum class GLResTypes {
        Shader, Program, VertexArrays, Buffer, Texture, FrameBuffer
        // ...
    };

    template<GLResTypes RT>
    struct GLRes {
        GLuint id{ (GLuint)-1 };
        operator GLuint const& () const { return id; }
        GLRes() = default;
        GLRes(GLuint id_) : id(id_) {}
        GLRes(GLRes const&) = delete;
        GLRes& operator=(GLRes const&) = delete;
        GLRes(GLRes&& o) noexcept { std::swap(id, o.id); }
        GLRes& operator=(GLRes&& o) noexcept { std::swap(id, o.id); return *this; }
        ~GLRes() {
            if (id == -1) return;
            if constexpr (RT == GLResTypes::Shader) glDeleteShader(id);
            if constexpr (RT == GLResTypes::Program) glDeleteProgram(id);
            if constexpr (RT == GLResTypes::VertexArrays) glDeleteVertexArrays(1, &id);
            if constexpr (RT == GLResTypes::Buffer) glDeleteBuffers(1, &id);
            if constexpr (RT == GLResTypes::Texture) glDeleteTextures(1, &id);
            if constexpr (RT == GLResTypes::FrameBuffer) glDeleteFramebuffers(1, &id);
            id = -1;
        }
    };

    using GLShader = GLRes<GLResTypes::Shader>;
    using GLProgram = GLRes<GLResTypes::Program>;
    using GLVertexArrays = GLRes<GLResTypes::VertexArrays>;
    using GLBuffer = GLRes<GLResTypes::Buffer>;
    using GLFrameBuffer = GLRes<GLResTypes::FrameBuffer>;
    struct GLTexture : GLRes<GLResTypes::Texture> {
        XY size{};
        UVRect Rect() const { return { 0,0, (uint16_t)size.x, (uint16_t)size.y }; }
        operator XY () const { return size; }
        operator XYu () const { return size; }
        operator UVRect () const { return Rect(); }

        static GLuint MakeTex();
        static void SetTexParm(GLuint id_, GLuint minFilter_, GLuint magFilter_, GLuint sWraper_, GLuint tWraper_); // filter:  GL_NEAREST  GL_LINEAR    wraper:  GL_CLAMP_TO_EDGE   GL_REPEAT

        void Make(XY size_, bool hasAlpha_ = true);
        void SetParm(GLuint minFilter_, GLuint magFilter_, GLuint sWraper_, GLuint tWraper_);
        void SetParm(GLuint minmagFilter_, GLuint stWraper_);
        void TryGenerateMipmap();
    };

    GLShader LoadGLShader(GLenum type, std::initializer_list<std::string_view>&& codes_);
    GLShader LoadGLVertexShader(std::initializer_list<std::string_view>&& codes_);
    GLShader LoadGLFragmentShader(std::initializer_list<std::string_view>&& codes_);
    GLProgram LinkGLProgram(GLuint vs, GLuint fs);
    GLTexture LoadGLTexture(void* data, GLsizei w, GLsizei h, GLint colorFormat);
    GLTexture LoadGLTexture(void* buf_, size_t len_);
    GLTexture LoadGLTexture(Span d);

    GLFrameBuffer MakeGLFrameBuffer();
    void BindGLFrameBufferTexture(GLuint f, GLuint t);
    void UnbindGLFrameBuffer();
    void GLFrameBufferSaveTo(Data& tar, GLint x, GLint y, GLsizei width, GLsizei height);   // only support GL_RGBA, GL_UNSIGNED_BYTE


    struct GLVertTexture : GLRes<GLResTypes::Texture> {
        XY size{};
        int32_t numVerts{}, numFrames{};
    };

    GLVertTexture LoadGLVertTexture(void* data, GLsizei w, GLsizei h, int32_t numVerts, int32_t numFrames);

    //struct GLTiledTexture : GLRes<GLResTypes::Texture> {
    //	XY size{};
    //	XYi sizeXY;
    //};

    //inline GLTiledTexture LoadGLTiledTexture(void* data, GLsizei w, GLsizei h, XYi size) {
    //	auto id = GLTexture::GenTex();
    //	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, data);
    //	return { id, {w, h}, size };
    //}
}
