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
	bool digging{};
	bool bouncing{};
	bool breaking{};
	Pickaxe pickaxe;	// digging coroutine
	void BeginDig();
	void BeginBounce();
	void Bounce();	// coroutine _2
	void Init(Scene_Test2* scene_);
	void Update();	// coroutine _1
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
	XY cRockMargin{};
	XY cRockMarginOffsetRange{};
	int32_t cRocksMaxCount{};
	float cRocksScale{};
	float cMouseCircleRadius{};
	XY cRocksPivotOffset{};
	int32_t rocksDisposedCountPerFrame{};

	xx::RGBA8 cBGColor{};
	float cBGColorplus{};
	float cBGTiling{};

	xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
	xx::Grid2dCircle<Rock2*> rocksGrid;		// life cycle: must upon rocks
	xx::List<xx::Shared<Rock2>> rocks;
	xx::Shared<xx::GLTexture> texBG;

	void GenRocks(int32_t count_);
	void SortRocks();

	void MakeUI();
	void Init(float totalScale_ = 1);
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
