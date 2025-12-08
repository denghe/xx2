#pragma once
#include "game.h"
#include "game_sceneitem.h"
#include "game_rock.h"

struct Porter : SceneItem {
	float radius{}, resRadius{}, speedScale{ 1 };
	bool flipX{};
	XY pos{}, catchingPos{}, rocksBasePos{};
	int32_t rocksMaxLen{};

	xx::List<CatchingRock> catchingRocks;
	xx::List<StackedRock> stackedRocks;
	xx::Weak<FlyingRock> target;
	XY targetPos{};
	float stepTime{};
	float catchRange{}, moveSpeed{};
	int32_t _1{};

	bool SearchTarget();
	void CatchFlyingRocks();
	bool MoveToTarget();
	void CalcCatchingPos();

	Porter& Init(Scene* scene_, XY pos_);
	bool Update() override;
	void Draw() override;
};
