#pragma once
#include "xx_includes.h"

namespace xx {
	
	struct STBImage {
		uint8_t* buf{};
		int32_t len{};
		int32_t w{}, h{}, comp{};

		void Fill(uint8_t const* buf_, size_t len_);
		void Fill(Span fd_);
		void Clear();
		XY Size() const;
		uint8_t& operator[](int32_t idx) const;
		RGBA8& At(int32_t idx) const;

		STBImage(uint8_t const* buf, size_t len);
		STBImage() = default;
		STBImage(STBImage const&) = delete;
		STBImage& operator=(STBImage const&) = delete;
		STBImage(STBImage&& o);
		STBImage& operator=(STBImage&& o);
		~STBImage();
	};

}
