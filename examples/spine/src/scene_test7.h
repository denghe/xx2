#pragma once
#include "game.h"

struct SpineFrameBatch {
	xx::Shared<xx::GLTexture> tex;
	XY size, anchor;	// , oPos, aPos
	int32_t numFrames{};
	int32_t numCols{}, numRows{};
	int32_t stepX{}, stepY{};
	xx::List<xx::TinyFrame> tfs;	// frame results
	void Init(spine::SkeletonData* sd_, spine::Animation* a_, XY scale_);
};

struct Scene_Test7;
struct Grass1 {
	Scene_Test7* scene{};
	SpineFrameBatch* sfb{};
	XY pos{}, scale{};
	int32_t frameIndex{};
	float colorPlus{};
	int32_t gridIndex{ -1 };
	void FillColorplus();
	void FillScale();
	void Init(Scene_Test7* scene_, SpineFrameBatch* sfb_, XY pos_);
	void InitGridIndex();
	void Update();
	void Draw();
	Grass1() = default;
	Grass1(Grass1 const&) = delete;
	Grass1& operator=(Grass1 const&) = delete;
	Grass1(Grass1&& o) noexcept;
	Grass1& operator=(Grass1&& o) noexcept;
	~Grass1();
};

struct Scene_Test7 : xx::SceneBase {
	static constexpr int32_t cNumMaxGlass{ 100000 };
	static constexpr int32_t cNumMaxLeaf{ 100000 };
	static constexpr float cMouseRadius{ 120 };

	xx::Shared<xx::Node> ui;

	xx::FromTo<float> cGrassScale{}, cGrassColorPlus{};
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

	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	SpineFrameBatch sfb;
	xx::Grid2dCircle<Grass1*> grid;	// life cycle: must upon grasses
	xx::List<Grass1> grasses;
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
