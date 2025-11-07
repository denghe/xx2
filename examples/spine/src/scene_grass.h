#pragma once
#include "game.h"
#include "spine_frame_batch.h"

struct Scene_Grass;
struct Grass {
	Scene_Grass* scene{};
	SpineFrameBatch* sfb{};
	XY pos{}, scale{};
	int32_t frameIndex{};
	float colorPlus{};
	int32_t gridIndex{ -1 };
	void FillColorplus();
	void FillScale();
	void Init(Scene_Grass* scene_, SpineFrameBatch* sfb_, XY pos_, bool randomFrameIndex_ = true);
	void InitGridIndex();
	void Update();
	void Draw();
	Grass() = default;
	Grass(Grass const&) = delete;
	Grass& operator=(Grass const&) = delete;
	Grass(Grass&& o) noexcept;
	Grass& operator=(Grass&& o) noexcept;
	~Grass();
};

struct Scene_Grass : xx::SceneBase {
	xx::Camera cam;
	xx::Grid2dCircle<Grass*> grid;	// life cycle: must upon grasses
	xx::FromTo<float> cGrassScale{}, cGrassColorPlus{};
};
