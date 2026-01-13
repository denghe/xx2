#pragma once
#include "xx_stbimage.h"
#include <stb_image.h>

namespace xx {
	
	void STBImage::Fill(uint8_t const* buf_, size_t len_) {
		if (buf) Clear();
		buf = (uint8_t*)stbi_load_from_memory((stbi_uc*)buf_, (int)len_, &w, &h, &comp, 0);
		len = comp * w * h;
	}

	void STBImage::Fill(Span fd_) {
		Fill(fd_.buf, fd_.len);
	}

	void STBImage::Clear() {
		if (buf) {
			stbi_image_free((stbi_uc*)buf);
			buf = {};
		}
	}

	XY STBImage::Size() const {
		return { w, h };
	}

	uint8_t& STBImage::operator[](int32_t idx) const {
		assert(buf);
		assert(idx >= 0);
		assert(len > idx);
		return ((uint8_t*)buf)[idx];
	}

	RGBA8& STBImage::At(int32_t idx) const {
		assert(comp == 4);
		assert(buf);
		assert(idx >= 0);
		assert(len > idx * comp);
		return ((RGBA8*)buf)[idx];
	}

	STBImage::STBImage(uint8_t const* buf, size_t len) {
		Fill(buf, len);
	}

	STBImage::STBImage(STBImage&& o) {
		operator=(std::move(o));
	}

	STBImage& STBImage::operator=(STBImage&& o) {
		std::swap(buf, o.buf);
		std::swap(len, o.len);
		std::swap(w, o.w);
		std::swap(h, o.h);
		std::swap(comp, o.comp);
		return *this;
	}

	STBImage::~STBImage() {
		Clear();
	}

}
