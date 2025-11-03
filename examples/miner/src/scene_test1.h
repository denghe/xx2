#pragma once
#include "game.h"

struct Rock {
	xx::TinyFrame tf;
	XY pos{}, fixedPos{};
};

struct Scene_Test1 : xx::SceneBase {
	static constexpr float cUIScale{ 0.5f };
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	static constexpr xx::FromTo<XYi> cGridSizeRange{ {0, 0}, { 160, 90 } };
	XYi cGridSize{ 16, 9 };
	xx::Shared<xx::Slider> uiGridSizeY;
	xx::Shared<xx::Slider> uiGridSizeX;

	static constexpr xx::FromTo<float> cRocksScaleRange{ 0.01, 1 };
	float cRocksScale{ 0.25f };
	xx::Shared<xx::Slider> uiRocksScale;

	xx::FromTo<int32_t> cRocksCountRange{ 0, cGridSize.x * cGridSize.y };
	int32_t cRocksCount{};
	xx::Shared<xx::Slider> uiRocksCount;

	// todo: random offset range set
	bool cEnableRandomOffset{};
	xx::Shared<xx::CheckBox> uiEnableRandomOffset;

	xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
	xx::Grid2dCircle<Rock*> rocksGrid;		// life cycle: must upon rocks
	xx::List<xx::Shared<Rock>> rocks;

	void GenRocksFixedPosPool();
	void GenRocks();
	void SetRandomOffset();
	void GenAll();

	void MakeUI();
	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
