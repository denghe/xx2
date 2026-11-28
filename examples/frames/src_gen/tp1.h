#pragma once
#include "pch.h"

struct Tp1 {
	// ../tp1/pics/a64.png
	static constexpr xx::FrameData _data_pics_a64_png{ xx::UVRect{68, 1, 19, 12}, xx::UVRect{38, 41, 64, 64}, xx::XY{0.4993684, 0.4486667/* src: 0.742, 0.256 */}, 1, 0 };
	// ../tp1/pics/p64.png
	static constexpr xx::FrameData _data_pics_p64_png{ xx::UVRect{1, 1, 64, 64}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FromTo<xx::XY> _scale9_pics_p64_png{ {1, 1}, {62, 62} };
	// ../tp1/pics/t64.png
	static constexpr xx::FrameData _data_pics_t64_png{ xx::UVRect{1, 68, 62, 53}, xx::UVRect{1, 11, 64, 64}, xx::XY{0.5, 0.9433962/* src: 0.5, 0.78125 */}, 1, 0 };
	xx::Frame pics_a64_png;
	xx::Frame pics_p64_png;
	xx::Frame pics_t64_png;

	xx::Shared<xx::GLTexture> Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true);
};
