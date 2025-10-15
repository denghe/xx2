#pragma once
#include "game.h"

struct SpineFrameBatch {
	xx::Ref<xx::GLTexture> tex;
	XY size, anchor;	// , oPos, aPos
	int32_t numFrames{};
	int32_t numCols{}, numRows{};
	int32_t stepX{}, stepY{};
	void Init(spine::SkeletonData* sd_, spine::Animation* a_);
	xx::UVRect GetUvRect(int32_t frameIndex_) const;
};

struct Scene_Test7;
struct Grass1 {
	Scene_Test7* scene{};
	XY pos{};
	float scale{ 1 };
	int32_t frameIndex{};
	void Init(Scene_Test7* scene_);
	void Update();
	void Draw();
};

struct Scene_Test7 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };
	SpineFrameBatch sfb;
	xx::List<Grass1> grasses;

	void Init();
	void FixedUpdate();
	void Update() override;
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
