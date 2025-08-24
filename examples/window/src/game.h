#pragma once
#include "pch.h"
using XY = xx::XY;

struct Heart;
struct Game : xx::Game<Game> {
	xx::Shared<xx::Node> ui;

	struct {
		xx::Ref<xx::Scale9Config> s9bN, s9bH;
	} cfgs;

	struct {
		xx::Ref<xx::GLTexture> heart, ui_button_n, ui_button_h;
	} texs;

	xx::Camera cam;
	xx::Shared<Heart> heart;

	void Init();
	void GLInit();
	void Update();
	void Stat();
	void OnResize();
};
extern Game gg;

struct Heart {
	static constexpr int32_t cAnimCount{ 2 };
	static constexpr std::array<xx::RGBA8, 4> cColors {
		xx::RGBA8_White, xx::RGBA8_Red, xx::RGBA8_Yellow, xx::RGBA8_Blue
	};

	XY scale{};
	int32_t colorIndex{}, animIndex{};

	void ChangeColor();
	void ChangeAnim();

	int32_t _1{};
	void AnimScale();

	int32_t _2{};
	float _2x{};
	void AnimBounce();

	void Init();
	void Update();
	void Draw();
};
