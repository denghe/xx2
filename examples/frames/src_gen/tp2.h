#pragma once
#include "pch.h"

struct Tp2 {
	static constexpr xx::FrameData _data_pics_barrel32{ xx::UVRect{68, 52, 40, 32}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FrameData _data_pics_circle32{ xx::UVRect{68, 87, 32, 32}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FrameData _data_pics_circle64{ xx::UVRect{1, 1, 64, 64}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FrameData _data_pics_p64{ xx::UVRect{1, 68, 64, 64}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FromTo<xx::XY> _scale9_pics_p64{ {1, 1}, {62, 62} };
	static constexpr xx::FrameData _data_pics_roundbox48{ xx::UVRect{68, 1, 48, 48}, xx::UVRect{}, xx::XY{0.5}, 1, 0 };
	static constexpr xx::FromTo<xx::XY> _scale9_pics_roundbox48{ {10, 10}, {28, 28} };
	static constexpr xx::FrameData _data_pics_t64{ xx::UVRect{1, 135, 62, 53}, xx::UVRect{1, 11, 64, 64}, xx::XY{0.5, 0.9433962/* src: 0.5, 0.78125 */}, 1, 0 };
	xx::Frame pics_barrel32;
	xx::Frame pics_circle32;
	xx::Frame pics_circle64;
	xx::Frame pics_p64;
	xx::Frame pics_roundbox48;
	xx::Frame pics_t64;

	xx::Shared<xx::GLTexture> Load(std::filesystem::path const& rootPath_, bool generateMipmap_ = true);
};
