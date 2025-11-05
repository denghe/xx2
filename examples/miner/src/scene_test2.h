#pragma once
#include "game.h"

struct Scene_Test2;
struct Rock2 {
	Scene_Test2* scene{};
	xx::TinyFrame tf;
	XY pos{}, fixedPos{}, centerPos{};
	int32_t indexAtGrid{-1};
	int32_t indexAtList{-1};	// fill by maker
	static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.25f) };
	float scale{};
	int32_t _1{};
	// todo: rock id ?
	void Init(Scene_Test2* scene_);
	void Update();
	void Draw();
	void Dispose();
	~Rock2();
};

struct Scene_Test2 : xx::SceneBase {
	static constexpr float cUIScale{ 0.5f };
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	XY cRockMarginOffsetRange{};
	int32_t cRocksMaxCount{};
	float cRocksScale{};
	float cMouseCircleRadius{};
	XY cRocksPivotOffset{};

	xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
	xx::Grid2dCircle<Rock2*> rocksGrid;		// life cycle: must upon rocks
	xx::List<xx::Shared<Rock2>> rocks;

	void GenRocks(int32_t count_);
	void SortRocks();

	void MakeUI();
	void Init(float totalScale_ = 1);
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
