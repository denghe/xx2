#pragma once
#include "pch.h"

struct Tp2 {
	// ../tp2/pics/barrel32.png
	static constexpr xx::FrameData _data_pics_barrel32_png{ xx::UVRect{68, 52, 40, 32}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	// ../tp2/pics/circle32.png
	static constexpr xx::FrameData _data_pics_circle32_png{ xx::UVRect{68, 87, 32, 32}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	// ../tp2/pics/circle64.png
	static constexpr xx::FrameData _data_pics_circle64_png{ xx::UVRect{1, 1, 64, 64}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	// ../tp2/pics/p64.png
	static constexpr xx::FrameData _data_pics_p64_png{ xx::UVRect{1, 68, 64, 64}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FromTo<xx::XY> _scale9_pics_p64_png{ {1, 1}, {62, 62} };
	// ../tp2/pics/roundbox48.png
	static constexpr xx::FrameData _data_pics_roundbox48_png{ xx::UVRect{68, 1, 48, 48}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FromTo<xx::XY> _scale9_pics_roundbox48_png{ {10, 10}, {28, 28} };
	// ../tp2/pics/t64.png
	static constexpr xx::FrameData _data_pics_t64_png{ xx::UVRect{1, 135, 62, 53}, xx::UVRect{1, 11, 64, 64}, xx::XY{0.5, 0.9433962/* src: 0.5, 0.78125 */}, 1, 0 };
	xx::Frame pics_barrel32_png;
	xx::Frame pics_circle32_png;
	xx::Frame pics_circle64_png;
	xx::Frame pics_p64_png;
	xx::Frame pics_roundbox48_png;
	xx::Frame pics_t64_png;

	xx::Shared<xx::GLTexture> Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true, bool pngAutoPremultiplyAlpla_ = true);
};
