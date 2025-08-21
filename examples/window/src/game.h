#pragma once
#include "pch.h"
using XY = xx::XY;

struct Game : xx::Game<Game> {
	xx::Shared<xx::Node> ui;

	struct {
	xx::Ref<xx::Scale9SpriteConfig> btnNormal, btnHighlight;
	} cfgs;

	struct {
	xx::RefGLTexture heart, ui_button_n, ui_button_h;
	} texs;

	struct {
	float scale{};
	int32_t colorIdx{};
	std::array<xx::RGBA8, 4> colors { xx::RGBA8_White, xx::RGBA8_Red, xx::RGBA8_Yellow, xx::RGBA8_Blue };
	} heart;

	void Init();
	void GLInit();
	xx::Task<> Task();
	void Update();
	void Stat();
	void OnResize();
};
extern Game gg;
