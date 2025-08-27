#pragma once
#include "game.h"

struct Scene_1;
struct Monster {
	static constexpr xx::FromTo<float> cAnimScaleRange{ .5f, 1.f };
	static constexpr float cAnimDuration{ 0.5f };
	static constexpr float cAnimStepDelay{ cAnimScaleRange.Sub() / (Game::cFps * cAnimDuration) };
	Scene_1* scene{};
	xx::Ref<xx::GLTexture> tex;
	XY pos{};
	float radius{};
	int32_t _1{};	// for coroutine
	float _1scale{};
	void Init(Scene_1* scene_, xx::Ref<xx::GLTexture> tex_, XY pos_, float radius_);
	void Update();
	void Draw();
};
