#pragma once
#include "game.h"
#include "spine_frame_batch.h"
#include "scene_grass.h"

struct Scene_Test8 : Scene_Grass {
	static constexpr float cMouseRadius{ 120 };

	xx::Shared<xx::Node> ui;

	int32_t cGrassCount{};
	bool cGrassRandomFrameIndex{};
	float cLeafScale{};
	float cLeafColorplus{};
	int32_t cLeafCount{};
	xx::RGBA8 cBGColor{};
	float cBGColorplus{};
	float cBGTiling{};

	static constexpr float cGrassRadius{ 64 };
	XY cGrassMarginOffsetRange{};
	int32_t cGrassMaxCount{};

	float time{}, timePool{}, timeScale{ 1 };
	SpineFrameBatch sfb;
	xx::List<XY> grassFixedPosPool;	// life cycle: must upon grasses
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
