#include "pch.h"
#include "tp2.h"

xx::Shared<xx::GLTexture> Tp2::Load(std::filesystem::path const& rootPath_, bool generateMipmap_) {
	auto t = xx::GameBase::instance->LoadTexture(xx::U8AsString((rootPath_ / "tp2.png").u8string()));
	if (generateMipmap_) t->TryGenerateMipmap();

	this->pics_barrel32.Fill(t, _data_pics_barrel32);
	this->pics_circle32.Fill(t, _data_pics_circle32);
	this->pics_circle64.Fill(t, _data_pics_circle64);
	this->pics_p64.Fill(t, _data_pics_p64);
	this->pics_roundbox48.Fill(t, _data_pics_roundbox48);
	this->pics_t64.Fill(t, _data_pics_t64);
	return t;
};
