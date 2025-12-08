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
	xx::Frame f;
	XY pos{}, centerPos{}, fixedPos{};
	int32_t indexAtGrid{ -1 };
	XY scale{};
	int32_t typeId{ -1 }, qualityId{ -1 };
	bool flip{};
	void Init(Scene* scene_);
	bool Update() override;
	void Draw() override;
	~BorningRock();	// try release fixedPos
};

struct Rock : BorningRock {
	static constexpr float cHPMax{ 100 };
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
	void Dispose();	// unsafe: release indexAtList
	void Init(BorningRock* src_);
	bool Update() override;	// coroutine _1
	void Draw() override;
	~Rock();	// try release indexAtGrid
};

struct BreakingRock : SceneItem {
	XY pos{};
	float breakFrameIndex{}, scale{};
	void Init(Rock* rock_);
	bool Update() override;
	void Draw() override;
};

struct Porter;
struct CollectingRock : SceneItem {
	int32_t indexAtGrid{ -1 };
	int32_t typeId{ -1 }, qualityId{ -1 };
	XY pos{};
	float alpha{};
	void Init(Rock* rock_);
	bool Update();
	void Draw() override;
	void Dispose();	// unsafe: release indexAtList
	void CatchBy(Porter* owner_);	// unsafe: release indexAtGrid & Dispose
	~CollectingRock();	// try release indexAtGrid
};

struct CatchingRock {
	int32_t typeId{ -1 }, qualityId{ -1 };
	XY pos{};
	float flySpeed{};
	void Init(CollectingRock* tar_);
	bool Update(Porter* porter_);
	void Draw(Scene* scene_);
};

struct StackedRock {
	int32_t typeId{ -1 }, qualityId{ -1 };
	XY pos{};
	XY CalcDrawPos(Porter* porter_) const;
	void Init(Porter* porter_, CatchingRock* catchingRock_);
	void Draw(Scene* scene_, XY p_);
};
