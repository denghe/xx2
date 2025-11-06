#pragma once
#include "game.h"

struct Scene_Test2;
struct Rock2;

// effect
struct Pickaxe {
	XY pos{};
	float radians{};
	int32_t _1{};
	void Init(Rock2* target_);
	bool Update();	// return true mean finished
	void Draw(Scene_Test2* scene_);
};

struct Rock2 {
	Scene_Test2* scene{};
	xx::TinyFrame tf;
	XY pos{}, centerPos{}, fixedPos{};
	int32_t indexAtGrid{-1};
	int32_t indexAtList{-1};	// fill by maker
	float scale{};
	// todo: rock id? value? hp?
	int32_t _1{};
	bool ready{};	// after born: true
	// todo: more state bool
	bool digging{};
	Pickaxe pickaxe;	// Update + Draw for digging
	// todo: shake, crash
	void Dig();
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

	static constexpr float cRockRadius{ 64 };
	XY cRockMarginOffsetRange{};
	int32_t cRocksMaxCount{};
	float cRocksScale{};
	float cMouseCircleRadius{};
	XY cRocksPivotOffset{};
	int32_t rocksDisposedCountPerFrame{};

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
