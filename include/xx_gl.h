#pragma once
#include "xx_prims.h"
#ifndef __EMSCRIPTEN__
#include <gl33.h>
#else
// todo
//#include <gles3.h>
#endif
#include <stb_image.h>

namespace xx {

#ifndef NDEBUG
	inline void CheckGLErrorAt(const char* file, int line, const char* func) {
		if (auto e = glGetError(); e != GL_NO_ERROR) {
			printf("glGetError() == %d file = %s line = %d\n", e, file, line);
			throw e;
		}
	}
#define CheckGLError() ::xx::CheckGLErrorAt(__FILE__, __LINE__, __FUNCTION__)
#else
#define CheckGLError() ((void)0)
#endif

	/**********************************************************************************************************************************/
	/**********************************************************************************************************************************/


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
		//std::string fileName;
		UVRect Rect() const {
			return { 0,0, (uint16_t)size.x, (uint16_t)size.y };
		}
		operator XY () const { return size; }
		operator XYu () const { return size; }
		operator UVRect () const { return Rect(); }

		inline static GLuint MakeTex() {
			GLuint id{};
			glGenTextures(1, &id);
			SetTexParm(id, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			return id;
		}

		// filter:  GL_NEAREST  GL_LINEAR    wraper:  GL_CLAMP_TO_EDGE   GL_REPEAT
		inline static void SetTexParm(GLuint id_, GLuint minFilter_, GLuint magFilter_, GLuint sWraper_, GLuint tWraper_) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, id_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWraper_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWraper_);
		}

		void Make(XY size_, bool hasAlpha_ = true) {
			assert(id == -1);
			id = MakeTex();
			auto c = hasAlpha_ ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, c, size_.x, size_.y, 0, c, GL_UNSIGNED_BYTE, {});
			size = size_;
		}

		void SetParm(GLuint minFilter_, GLuint magFilter_, GLuint sWraper_, GLuint tWraper_) {
			SetTexParm(id, minFilter_, magFilter_, sWraper_, tWraper_);
		}

		void SetParm(GLuint minmagFilter_, GLuint stWraper_) {
			SetTexParm(id, minmagFilter_, minmagFilter_, stWraper_, stWraper_);
		}

		void TryGenerateMipmap() {
			if (size.x == size.y && Round2n((size_t)size.x) == (size_t)size.x) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, id);
				glGenerateMipmap(GL_TEXTURE_2D);
				CheckGLError();
			}
		}
	};

	struct STBImage {
		uint8_t* buf{};
		int32_t len{};
		int32_t w{}, h{}, comp{};

		void Fill(uint8_t const* buf_, size_t len_) {
			if (buf) Clear();
			buf = (uint8_t*)stbi_load_from_memory((stbi_uc*)buf_, (int)len_, &w, &h, &comp, 0);
			len = comp * w * h;
		}
		void Fill(std::pair<Data, std::string> const& fd) {
			Fill(fd.first.buf, fd.first.len);
		}
		void Clear() {
			if (buf) {
				stbi_image_free((stbi_uc*)buf);
				buf = {};
			}
		}
		XY Size() const {
			return { w, h };
		}

		uint8_t& operator[](int32_t idx) const {
			assert(buf);
			assert(idx >= 0);
			assert(len > idx);
			return ((uint8_t*)buf)[idx];
		}

		RGBA8& At(int32_t idx) const {
			assert(comp == 4);
			assert(buf);
			assert(idx >= 0);
			assert(len > idx * comp);
			return ((RGBA8*)buf)[idx];
		}

		STBImage(uint8_t const* buf, size_t len) {
			Fill(buf, len);
		}
		STBImage() = default;
		STBImage(STBImage const&) = delete;
		STBImage& operator=(STBImage const&) = delete;
		STBImage(STBImage&& o) {
			operator=(std::move(o));
		}
		STBImage& operator=(STBImage&& o) {
			std::swap(buf, o.buf);
			std::swap(len, o.len);
			std::swap(w, o.w);
			std::swap(h, o.h);
			std::swap(comp, o.comp);
			return *this;
		}
		~STBImage() {
			Clear();
		}
	};

	/**********************************************************************************************************************************/
	/**********************************************************************************************************************************/

	inline GLShader LoadGLShader(GLenum type, std::initializer_list<std::string_view>&& codes_) {
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
			CoutN("glCompileShader failed: err msg = ", s);
			throw s;
		}
		return GLShader(shader);
	}

	inline GLShader LoadGLVertexShader(std::initializer_list<std::string_view>&& codes_) {
		return LoadGLShader(GL_VERTEX_SHADER, std::move(codes_));
	}

	inline GLShader LoadGLFragmentShader(std::initializer_list<std::string_view>&& codes_) {
		return LoadGLShader(GL_FRAGMENT_SHADER, std::move(codes_));
	}

	inline GLProgram LinkGLProgram(GLuint vs, GLuint fs) {
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
			CoutN("glLinkProgram failed: err msg = ", s);
			throw s;
		}
		return GLProgram(program);
	}

	/**********************************************************************************************************************************/
	/**********************************************************************************************************************************/

	inline GLFrameBuffer MakeGLFrameBuffer() {
		GLuint f{};
		glGenFramebuffers(1, &f);
		//glBindFramebuffer(GL_FRAMEBUFFER, f);
		return GLFrameBuffer(f);
	}

	inline void BindGLFrameBufferTexture(GLuint f, GLuint t) {
		glBindFramebuffer(GL_FRAMEBUFFER, f);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	inline void UnbindGLFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// only support GL_RGBA, GL_UNSIGNED_BYTE
	inline void GLFrameBufferSaveTo(Data& tar, GLint x, GLint y, GLsizei width, GLsizei height) {
		auto siz = width * height * 4;
		tar.Resize(siz);
		glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tar.buf);
	}


	/**********************************************************************************************************************************/
	/**********************************************************************************************************************************/

	// data's bytes len == w * h * sizeof(colorFormat)
	inline GLTexture LoadGLTexture(void* data, GLsizei w, GLsizei h, GLint colorFormat, std::string_view fn) {
		auto id = GLTexture::MakeTex();
		if (colorFormat == GL_RGBA) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 8 - 4 * (w & 0x1));
		}
		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, w, h, 0, colorFormat, GL_UNSIGNED_BYTE, data);
		CheckGLError();
		return { id, {w, h}/*, std::string(fn)*/ };
	}


	inline GLTexture LoadGLTexture(std::string_view buf, std::string_view fullPath) {
		assert(buf.size() >= 16);

		// png
		if (buf.starts_with("\x89\x50\x4e\x47\x0d\x0a\x1a\x0a"sv)) {
			int w, h, comp;
			if (auto image = stbi_load_from_memory((stbi_uc*)buf.data(), (int)buf.size(), &w, &h, &comp, 0)) {
				assert(comp == 3 || comp == 4);
				auto t = LoadGLTexture(image, w, h, comp == 3 ? GL_RGB : GL_RGBA, fullPath);
				stbi_image_free(image);
				return t;
			}
		}

		// jpg
		else if (buf.starts_with("\xFF\xD8"sv)) {
			int w, h, comp;
			if (auto image = stbi_load_from_memory((stbi_uc*)buf.data(), (int)buf.size(), &w, &h, &comp, 0)) {
				assert(comp == 3 || comp == 4);
				auto t = LoadGLTexture(image, w, h, GL_RGB, fullPath);
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

		CoutN("failed to load texture. fn = ", fullPath);
		throw fullPath;
		return {};
	}

	// ...

	/**********************************************************************************************************************************/
	/**********************************************************************************************************************************/

	struct GLVertTexture : GLRes<GLResTypes::Texture> {
		XY size{};
		int32_t numVerts{}, numFrames{};
	};

	inline GLVertTexture LoadGLVertTexture(void* data, GLsizei w, GLsizei h, int32_t numVerts, int32_t numFrames) {
		auto id = GLTexture::MakeTex();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, data);
		return { id, { w, h }, numVerts, numFrames };
	}

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
