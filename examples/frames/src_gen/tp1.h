#pragma once
#include <xx_frame.h>

struct Tp1 {
	xx::Frame pics_a64;
	xx::Frame pics_p64;
	xx::Frame pics_t64;
	static constexpr xx::FromTo<xx::XY> _scale9_pics_p64{ {1, 1}, {62, 62} };

	xx::Shared<xx::GLTexture> Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true);
};
