#pragma once
#include "game.h"
#include "game_sceneitem.h"

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

struct BorningRock : SceneItem {
	xx::TinyFrame tf;
	XY pos{}, centerPos{}, fixedPos{};
	int32_t indexAtGrid{ -1 };
	XY scale{};
	int32_t typeId{ -1 }, qualityId{ -1 };
	bool flip{};
	void Init(Scene* scene_);
	bool Update() override;
	void Draw() override;
	~BorningRock();
};

struct Rock : BorningRock {
	static constexpr float cHPMax{ 100 };
	int32_t indexAtList{ -1 };
	int32_t hp{};
	int32_t _1{}, _2{};
	float whiteEndTime{};
	bool digging{};
	bool bouncing{};
	Pickaxe pickaxe;	// digging coroutine
	bool Hit(int32_t dmg_);	// return true: no hp
	void BeginWhite();
	void BeginDig();
	void BeginBounce();
	void Bounce();	// coroutine _2
	void Break();
	void Dispose();	// unsafe: Release this
	void Init(BorningRock* src_);
	bool Update() override;	// coroutine _1
	void Draw() override;
	~Rock();
};

struct BreakingRock : SceneItem {
	XY pos{};
	float breakFrameIndex{}, scale{};
	void Init(Rock* rock_);
	bool Update() override;
	void Draw() override;
};

struct FlyingRock {
	static constexpr float cFlySpeed{ 1200 / gg.cFps };
	static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.6f) };
	Scene* scene{};
	XY pos{}, inc{};
	float scale{}, scaleStep{}, moveCount{}, moveIndex{};
	int32_t typeId{ -1 }, qualityId{ -1 };
	int32_t _1{};
	void Init(Rock* rock_);
	bool Update();	// coroutine _1
	void Draw(Scene* scene_);
};
