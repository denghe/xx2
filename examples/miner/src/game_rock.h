#pragma once
#include "game.h"

struct Scene;
struct Rock;

struct Pickaxe {
	XY pos{};
	float radians{};
	int32_t _1{};
	void Init(Rock* target_);
	bool Update();	// return true mean finished
	void Draw(Scene* scene_);
};

struct Rock {
	Scene* scene{};
	xx::TinyFrame tf;
	XY pos{}, centerPos{}, fixedPos{};
	int32_t indexAtGrid{ -1 };
	int32_t indexAtList{ -1 };	// fill by maker
	XY scale{};
	int32_t typeId{ -1 }, qualityId{ -1 };
	static constexpr float cHPMax{ 100 };
	int32_t hp{};
	int32_t _1{}, _2{};
	float breakFrameIndex{};
	bool flip{};
	bool ready{};	// true: can dig
	bool mining{};
	bool bouncing{};
	bool breaking{};
	Pickaxe pickaxe;	// mining coroutine
	void BeginDig();
	void BeginBounce();
	void Bounce();	// coroutine _2
	void Init(Scene* scene_);
	void Update();	// coroutine _1
	void Draw();
	void Dispose();
	~Rock();
};
