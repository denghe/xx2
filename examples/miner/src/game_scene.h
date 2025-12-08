#pragma once
#include "game.h"
#include "game_sceneitem.h"
#include "game_rock.h"
#include "game_miner.h"
#include "game_porter.h"
//#include "game_gambler.h"
#include "game_map.h"
#include "game_minecart.h"

struct Scene : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	std::array<int32_t, 9> counts{};
	std::array<xx::Weak<xx::ImageLabelButton>, 9> countUIs;

	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 }, timer{};

	static constexpr float cRockRadius{ 64 };
	XY cRockMargin{};
	XY cRockMarginOffsetRange{};
	int32_t cRocksMaxCount{};
	float cRocksScale{};
	float cMouseCircleRadius{};
	XY cRocksPivotOffset{};
	int32_t rocksDisposedCountPerFrame{};

	xx::Shared<Map> map;
	xx::Shared<MineCart> minecart;
	xx::List<xx::Shared<Miner>> miners;
	xx::List<xx::Shared<Porter>> porters;
	//xx::List<xx::Shared<Gambler>> gamblers;

	xx::List<XY> rocksFixedPoss;					// for miner pos
	xx::List<XY> rocksFixedPosPool;
	xx::List<xx::Shared<BorningRock>> borningRocks;	// life cycle: must below rocksFixedPosPool

	xx::Grid2dCircle<Rock*> rocksGrid;
	xx::List<xx::Shared<Rock>> rocks;				// life cycle: must below rocksGrid

	xx::List<BreakingRock> breakingRocks;

	xx::Grid2dCircle<FlyingRock*> flyingRocksGrid;
	xx::List<xx::Shared<FlyingRock>> flyingRocks;	// life cycle: must below flyingRocksGrid

	xx::List<SceneItem*> sortContainer;				// for draw order by Y
	void SortContainerAdd(SceneItem* o);
	void SortContainerDraw();

	void GenMiners(int32_t count_);
	void GenPorters(int32_t count_);
	void GenRocks(int32_t count_);

	virtual void MakeUI();
	void Init(float totalScale_ = 1);
	void Update() override;
	virtual void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
	void OnFocus(bool focused_) override;
};
