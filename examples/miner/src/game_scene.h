#pragma once
#include "game.h"
#include "game_sceneitem.h"
#include "game_rock.h"
#include "game_monster.h"

struct Scene : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	xx::RGBA8 cBGColor{};
	float cBGColorplus{};
	float cBGTiling{};

	static constexpr float cRockRadius{ 64 };
	XY cRockMargin{};
	XY cRockMarginOffsetRange{};
	int32_t cRocksMaxCount{};
	float cRocksScale{};
	float cMouseCircleRadius{};
	XY cRocksPivotOffset{};
	int32_t rocksDisposedCountPerFrame{};

	xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
	xx::Grid2dCircle<Rock*> rocksGrid;		// life cycle: must upon rocks
	xx::List<xx::Shared<Rock>> rocks;
	std::array<XY, 9> flyTargets{};
	std::array<int32_t, 9> counts{};
	std::array<xx::Weak<xx::ImageLabelButton>, 9> countUIs;
	xx::List<FlyingRock> flyingRocks;
	xx::List<xx::Shared<Monster>> monsters;
	xx::List<xx::Shared<BorningRock>> borningRocks;	// life cycle: must below rocks
	xx::List<BreakingRock> breakingRocks;	// effect
	xx::List<std::pair<float, SceneItem*>> sitems;	// for draw order

	void GenRocks(int32_t count_);
	void SortRocks();
	void GenMonsters(int32_t count_);

	virtual void MakeUI();
	void Init(float totalScale_ = 1);
	void Update() override;
	virtual void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
