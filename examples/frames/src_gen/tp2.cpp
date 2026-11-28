#include "pch.h"
#include "tp2.h"

xx::Shared<xx::GLTexture> Tp2::Load(std::filesystem::path const& rootPath_, bool generateMipmap_, bool pngAutoPremultiplyAlpla_) {
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString((rootPath_ / "tp2.png").u8string()), pngAutoPremultiplyAlpla_);
	if (generateMipmap_) t->TryGenerateMipmap();

	this->pics_barrel32_png.Fill(t, _data_pics_barrel32_png);
	this->pics_circle32_png.Fill(t, _data_pics_circle32_png);
	this->pics_circle64_png.Fill(t, _data_pics_circle64_png);
	this->pics_p64_png.Fill(t, _data_pics_p64_png);
	this->pics_roundbox48_png.Fill(t, _data_pics_roundbox48_png);
	this->pics_t64_png.Fill(t, _data_pics_t64_png);
	return t;
};
