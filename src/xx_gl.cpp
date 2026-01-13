#include <xx.h>
//#include <fstream>
//#include <zstd.h>

//#define STBI_NO_JPEG
//#define STBI_NO_PNG
#define STBI_NO_GIF
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_HDR
#define STBI_NO_TGA
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb_image_write.h>
//
//#define STB_RECT_PACK_IMPLEMENTATION
//#include <stb_rect_pack.h>

namespace xx {
	
    /**************************************************************************************************/
    // xx_gl.h
    /**************************************************************************************************/

#ifndef NDEBUG
    void CheckGLErrorAt(const char* file, int line, const char* func) {
        if (auto e = glGetError(); e != GL_NO_ERROR) {
            printf("glGetError() == %d file = %s line = %d\n", e, file, line);
            throw e;
        }
    }
#endif

    GLuint GLTexture::MakeTex() {
        GLuint id{};
        glGenTextures(1, &id);
        SetTexParm(id, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        return id;
    }

    // filter:  GL_NEAREST  GL_LINEAR    wraper:  GL_CLAMP_TO_EDGE   GL_REPEAT
    void GLTexture::SetTexParm(GLuint id_, GLuint minFilter_, GLuint magFilter_, GLuint sWraper_, GLuint tWraper_) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWraper_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWraper_);
    }

    void GLTexture::Make(XY size_, bool hasAlpha_) {
        assert(id == -1);
        id = MakeTex();
        auto c = hasAlpha_ ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, c, size_.x, size_.y, 0, c, GL_UNSIGNED_BYTE, {});
        size = size_;
    }

    void GLTexture::SetParm(GLuint minFilter_, GLuint magFilter_, GLuint sWraper_, GLuint tWraper_) {
        SetTexParm(id, minFilter_, magFilter_, sWraper_, tWraper_);
    }

    void GLTexture::SetParm(GLuint minmagFilter_, GLuint stWraper_) {
        SetTexParm(id, minmagFilter_, minmagFilter_, stWraper_, stWraper_);
    }

    void GLTexture::TryGenerateMipmap() {
        if (size.x == size.y && Round2n((size_t)size.x) == (size_t)size.x) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);
            glGenerateMipmap(GL_TEXTURE_2D);
            CheckGLError();
        }
    }

    GLShader LoadGLShader(GLenum type, std::initializer_list<std::string_view>&& codes_) {
        assert(codes_.size() && (type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER));
        auto&& shader = glCreateShader(type);
        assert(shader);
        std::vector<GLchar const*> codes;
        codes.resize(codes_.size());
        std::vector<GLint> codeLens;
        codeLens.resize(codes_.size());
        auto ss = codes_.begin();
        for (size_t i = 0; i < codes.size(); ++i) {
            codes[i] = (GLchar const*)ss[i].data();
            codeLens[i] = (GLint)ss[i].size();
        }
        glShaderSource(shader, (GLsizei)codes_.size(), codes.data(), codeLens.data());
        glCompileShader(shader);
        GLint r = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &r);
        if (!r) {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &r);	// fill txt len into r
            std::string s;
            if (r) {
                s.resize(r);
                glGetShaderInfoLog(shader, r, nullptr, s.data());	// copy txt to s
            }
            printf("glCompileShader failed: err msg = %s", s.c_str());
            throw s;
        }
        return GLShader(shader);
    }

    GLShader LoadGLVertexShader(std::initializer_list<std::string_view>&& codes_) {
        return LoadGLShader(GL_VERTEX_SHADER, std::move(codes_));
    }

    GLShader LoadGLFragmentShader(std::initializer_list<std::string_view>&& codes_) {
        return LoadGLShader(GL_FRAGMENT_SHADER, std::move(codes_));
    }

    GLProgram LinkGLProgram(GLuint vs, GLuint fs) {
        auto program = glCreateProgram();
        assert(program);
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        GLint r = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &r);
        if (!r) {
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &r);
            std::string s;
            if (r) {
                s.resize(r);
                glGetProgramInfoLog(program, r, nullptr, s.data());
            }
            printf("glLinkProgram failed: err msg = %s", s.c_str());
            throw s;
        }
        return GLProgram(program);
    }


    // data's bytes len == w * h * sizeof(colorFormat)
    GLTexture LoadGLTexture(void* data, GLsizei w, GLsizei h, GLint colorFormat) {
        auto id = GLTexture::MakeTex();
        if (colorFormat == GL_RGBA) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 8 - 4 * (w & 0x1));
        }
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, w, h, 0, colorFormat, GL_UNSIGNED_BYTE, data);
        CheckGLError();
        return { id, {w, h} };
    }


    GLTexture LoadGLTexture(void* buf_, size_t len_) {
        assert(len_ >= 16);
        std::string_view buf((char*)buf_, len_);

        // png
        if (buf.starts_with("\x89\x50\x4e\x47\x0d\x0a\x1a\x0a"sv)) {
            int w, h, comp;
            if (auto image = stbi_load_from_memory((stbi_uc*)buf.data(), (int)buf.size(), &w, &h, &comp, 0)) {
                assert(comp == 3 || comp == 4);
                auto t = LoadGLTexture(image, w, h, comp == 3 ? GL_RGB : GL_RGBA);
                stbi_image_free(image);
                return t;
            }
        }

        // jpg
        else if (buf.starts_with("\xFF\xD8"sv)) {
            int w, h, comp;
            if (auto image = stbi_load_from_memory((stbi_uc*)buf.data(), (int)buf.size(), &w, &h, &comp, 0)) {
                assert(comp == 3 || comp == 4);
                auto t = LoadGLTexture(image, w, h, GL_RGB);
                stbi_image_free(image);
                return t;
            }
        }

        // pvr3 with Etc2_RGBA
        // todo: soft decompress for MACOS
        /* https://github.com/wolfpld/etcpak.git
        * BlockData.cpp / WritePvrHeader
    uint32_t* dst;
    *dst++ = 0x03525650;  // version				// PVR\x3
    *dst++ = 0;           // flags
    switch( type ) {      // pixelformat[0]
    case CodecType::Etc2_RGB:        *dst++ = 22;        break;
    case CodecType::Etc2_RGBA:        *dst++ = 23;        break;
    case CodecType::Etc2_R11:        *dst++ = 25;        break;
    case CodecType::Etc2_RG11:        *dst++ = 26;        break;
        ........
    }
    *dst++ = 0;           // pixelformat[1]
    *dst++ = 0;           // colourspace
    *dst++ = 0;           // channel type
    *dst++ = size.y;      // height
    *dst++ = size.x;      // width
    *dst++ = 1;           // depth
    *dst++ = 1;           // num surfs
    *dst++ = 1;           // num faces
    *dst++ = levels;      // mipmap count
    *dst++ = 0;           // metadata size

        */
        else if (buf.starts_with("PVR\x3"sv)) {
            auto p = (uint32_t*)buf.data();
            ++p;				// version
            assert(*p == 0);	// flags
            auto format = *++p;		// GL_COMPRESSED_RGB8_ETC2: 0x9274
            assert(format == 23);	// GL_COMPRESSED_RGBA8_ETC2_EAC: 0x9278
            p += 3;				// pixelformat[1], colourspace, channel type
            auto height = *++p;
            auto width = *++p;
            p += 3;				// depth, num surfs, num faces
            auto levels = *++p;
            ++p;				// metadata size
            ++p;
            //auto dataLen = buf.size() - ((char*)p - buf.data());
            auto dataLen = width * height;
            auto ptr = (char*)p;

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 8 - 4 * (width & 0x1));
            auto id = GLTexture::MakeTex();
            for (uint32_t i = 0; i < levels; ++i) {
                glCompressedTexImage2D(GL_TEXTURE_2D, (GLint)i, 0x9278, (GLsizei)width, (GLsizei)height, 0, (GLsizei)dataLen, ptr);
                CheckGLError();
                ptr += dataLen;
                assert(ptr <= buf.data() + buf.size());
                dataLen /= 4;
                width /= 2;
                height /= 2;
                if (width < 4 || height < 4) break;
            }
            //glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            CheckGLError();
            return { id, {width, height}/*, fullPath*/ };
        }

        // ...

        throw __LINE__;
        return {};
    }

    GLTexture LoadGLTexture(Span d) {
        return LoadGLTexture(d.buf, d.len);
    }

    GLFrameBuffer MakeGLFrameBuffer() {
        GLuint f{};
        glGenFramebuffers(1, &f);
        //glBindFramebuffer(GL_FRAMEBUFFER, f);
        return GLFrameBuffer(f);
    }

    void BindGLFrameBufferTexture(GLuint f, GLuint t) {
        glBindFramebuffer(GL_FRAMEBUFFER, f);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    void UnbindGLFrameBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // only support GL_RGBA, GL_UNSIGNED_BYTE
    void GLFrameBufferSaveTo(Data& tar, GLint x, GLint y, GLsizei width, GLsizei height) {
        tar.Resize(width * height * 4);
        glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tar.buf);
    }



    GLVertTexture LoadGLVertTexture(void* data, GLsizei w, GLsizei h, int32_t numVerts, int32_t numFrames) {
        auto id = GLTexture::MakeTex();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, data);
        return { id, { w, h }, numVerts, numFrames };
    }

}
