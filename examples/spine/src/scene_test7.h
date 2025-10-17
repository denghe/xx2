#pragma once
#include "game.h"

struct SpineFrameBatch {
	xx::Ref<xx::GLTexture> tex;
	XY size, anchor;	// , oPos, aPos
	int32_t numFrames{};
	int32_t numCols{}, numRows{};
	int32_t stepX{}, stepY{};
	void Init(spine::SkeletonData* sd_, spine::Animation* a_, XY scale_);
	xx::UVRect GetUvRect(int32_t frameIndex_) const;
};

struct Scene_Test7;
struct Grass1 {
	Scene_Test7* scene{};
	XY pos{}, scale{};
	int32_t frameIndex{};
	float colorPlus{};
	void FillColorplus();
	void FillScale();
	void Init(Scene_Test7* scene_, XY pos_);
	void Update();
	void Draw();
};

struct Scene_Test7 : xx::SceneBase {
	xx::Shared<xx::Node> ui;

	xx::FromTo<float> cGrassScale{}, cGrassColorPlus{};
	xx::Shared<xx::Slider> uiGrassScaleFrom, uiGrassScaleTo, uiGrassColorPlusFrom, uiGrassColorPlusTo;

	// todo: grass color?
	static constexpr xx::FromTo<int32_t> cGrassCountRange{ 0, 100000 };
	int32_t cGrassCount{};
	xx::Shared<xx::Slider> uiGrassCount;

	static constexpr xx::FromTo<float> cLeafScaleRange{ 0.01, 1 };
	float cLeafScale{};
	xx::Shared<xx::Slider> uiLeafScale;

	static constexpr xx::FromTo<float> cLeafColorplusRange{ 0, 10 };
	float cLeafColorplus{};
	xx::Shared<xx::Slider> uiLeafColorplus;

	static constexpr xx::FromTo<int32_t> cLeafCountRange{ 0, 100000 };
	int32_t cLeafCount{};
	xx::Shared<xx::Slider> uiLeafCount;

	// todo: bg color?
	static constexpr xx::FromTo<float> cBGColorplusRange{ 0, 10 };
	float cBGColorplus{};
	xx::Shared<xx::Slider> uiBGColorplus;

	static constexpr xx::FromTo<float> cBGTilingRange{ 0.15, 3 };
	float cBGTiling{};
	xx::Shared<xx::Slider> uiBGTiling;

	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	SpineFrameBatch sfb;
	xx::List<Grass1> grasses;
	xx::Ref<xx::GLTexture> texBG, texLeaf;
	xx::FromTo<float> xRange, yRange;

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
