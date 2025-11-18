#pragma once
#include "game.h"

struct Scene;
struct Rock;

struct FlyingRock {
	static constexpr float cFlySpeed{ 1200 / gg.cFps };
	static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.6f) };
	Scene* scene{};
	XY pos{}, inc{};
	float scale{}, scaleStep{}, moveCount{}, moveIndex{};
	int32_t typeId{};
	int32_t _1{};
	void Init(Rock* rock_);
	bool Update();	// coroutine _1
	void Draw(Scene* scene_);
};
