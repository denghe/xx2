#pragma once
#include "game.h"
#include "spine_frame_batch.h"
#include "scene_grass.h"

struct Scene_Test7 : Scene_Grass {
	static constexpr int32_t cNumMaxGlass{ 100000 };
	static constexpr int32_t cNumMaxLeaf{ 100000 };
	static constexpr float cMouseRadius{ 120 };

	xx::Shared<xx::Node> ui;

	//xx::FromTo<float> cGrassScale{}, cGrassColorPlus{};
	xx::Shared<xx::Slider> uiGrassScaleFrom, uiGrassScaleTo, uiGrassColorPlusFrom, uiGrassColorPlusTo;

	// todo: grass color?
	static constexpr xx::FromTo<int32_t> cGrassCountRange{ 0, cNumMaxGlass };
	int32_t cGrassCount{};
	xx::Shared<xx::Slider> uiGrassCount;

	static constexpr xx::FromTo<float> cLeafScaleRange{ 0.01, 1 };
	float cLeafScale{};
	xx::Shared<xx::Slider> uiLeafScale;

	static constexpr xx::FromTo<float> cLeafColorplusRange{ 0, 10 };
	float cLeafColorplus{};
	xx::Shared<xx::Slider> uiLeafColorplus;

	static constexpr xx::FromTo<int32_t> cLeafCountRange{ 0, cNumMaxLeaf };
	int32_t cLeafCount{};
	xx::Shared<xx::Slider> uiLeafCount;

	// todo: bg color?
	static constexpr xx::FromTo<float> cBGColorplusRange{ 0, 10 };
	float cBGColorplus{};
	xx::Shared<xx::Slider> uiBGColorplus;

	static constexpr xx::FromTo<float> cBGTilingRange{ 0.15, 3 };
	float cBGTiling{};
	xx::Shared<xx::Slider> uiBGTiling;

	//xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	SpineFrameBatch sfb;
	//xx::Grid2dCircle<Grass*> grid;	// life cycle: must upon grasses
	xx::List<Grass> grasses;
	xx::Shared<xx::GLTexture> texBG, texLeaf;

	xx::List<SpineFrameBatch> sfbsFlower;
	xx::List<SpineFrameBatch> sfbsGrass;


	void GenGrass();
	void GenLeaf();
	void GenBG();
	void ChangeGrassColorplus();
	void ChangeGrassScale();

	void Init();
	void FixedUpdate();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
