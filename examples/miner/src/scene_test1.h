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

	static constexpr XYi cGridSize{ 40, 20 };

	static constexpr xx::FromTo<float> cRocksScaleRange{ 0.01, 1 };
	float cRocksScale{ 0.25f };
	xx::Shared<xx::Slider> uiRocksScale;

	static constexpr xx::FromTo<int32_t> cRocksCountRange{ 0, cGridSize.x * cGridSize.y };
	int32_t cRocksCount{ 100 };
	xx::Shared<xx::Slider> uiRocksCount;

	xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
	xx::Grid2dCircle<Rock*> rocksGrid;		// life cycle: must upon rocks
	xx::List<xx::Shared<Rock>> rocks;

	void GenRocks();
	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
