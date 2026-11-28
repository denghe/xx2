#include "pch.h"
#include "tp1.h"

xx::Shared<xx::GLTexture> Tp1::Load(std::filesystem::path const& rootPath_, bool generateMipmap_, bool pngAutoPremultiplyAlpla_) {
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString((rootPath_ / "tp1.png").u8string()), pngAutoPremultiplyAlpla_);
	if (generateMipmap_) t->TryGenerateMipmap();

	this->pics_a64_png.Fill(t, _data_pics_a64_png);
	this->pics_p64_png.Fill(t, _data_pics_p64_png);
	this->pics_t64_png.Fill(t, _data_pics_t64_png);
	return t;
};
