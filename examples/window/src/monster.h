#pragma once
#include "game.h"

struct Monster {
	static constexpr int32_t cAnimCount{ 2 };
	static constexpr std::array<xx::RGBA8, 4> cColors {
		xx::RGBA8_White, xx::RGBA8_Red, xx::RGBA8_Yellow, xx::RGBA8_Blue
	};

	xx::Frame frame;
	XY scale{};
	int32_t colorIndex{}, animIndex{};

	void ChangeColor();
	void ChangeAnim();

	int32_t _1{};
	void AnimScale();

	int32_t _2{};
	float _2x{};
	void AnimBounce();

	void Init(xx::Frame tex_);
	void Update();
	void Draw();
};
