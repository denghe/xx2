#pragma once
#include "game.h"

struct Scene_Play;
struct Monster {
	static constexpr xx::FromTo<float> cAnimScaleRange{ .5f, 1.f };
	static constexpr float cAnimDuration{ 0.5f };
	static constexpr float cAnimStepDelay{ cAnimScaleRange.Sub() / (Game::cFps * cAnimDuration) };
	Scene_Play* scene{};
	xx::TinyFrame tf;
	XY pos{}, ori{};
	float radius{};

	int32_t _1{};	// for coroutine
	float _1scale{};
	void Idle_1();

	xx::Shaker shaker;
	bool shaking{};

	void Init(Scene_Play* scene_, XY pos_, float radius_);
	void Update();
	void Draw();
};
