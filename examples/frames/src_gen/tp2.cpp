#include <xx_gamebase.h>
#include "tp2.h"

xx::Shared<xx::GLTexture> Tp2::Load(std::filesystem::path const& rootPath_, bool generateMipmap_) {
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString((rootPath_ / "tp2.png").u8string()));
	if (generateMipmap_) t->TryGenerateMipmap();

	static constexpr xx::UVRect uvrects[] = {
	{ 1, 1, 64, 64 },
	{ 1, 68, 62, 53 },
	};

	static constexpr xx::UVRect uvtrims[] = {
	{}, 
	{ 1, 11, 64, 64 },
	};

	static constexpr xx::XY anchors[] = {
	{ 0.5 },
	{ 0.5, 0.78125 },
	};

	for (int32_t i = 0; i < 2; i++) {
		auto& f = ((xx::Frame*)this)[i];
		f.tex = t;
		f.uvRect = uvrects[i];
		f.uvTrim = uvtrims[i];
		f.anchor = anchors[i];
		f.extrude = 1;
	}

	return t;
};
