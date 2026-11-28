#include "pch.h"
#include "tp1.h"

xx::Shared<xx::GLTexture> Tp1::Load(std::filesystem::path const& rootPath_, bool generateMipmap_) {
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString((rootPath_ / "tp1.png").u8string()));
	if (generateMipmap_) t->TryGenerateMipmap();

	this->pics_a64.Fill(t, _data_pics_a64);
	this->pics_p64.Fill(t, _data_pics_p64);
	this->pics_t64.Fill(t, _data_pics_t64);
	return t;
};
