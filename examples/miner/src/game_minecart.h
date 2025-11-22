#pragma once
#include "game.h"
#include "game_sceneitem.h"

struct MineCart : SceneItem {
	xx::Shared<xx::GLTexture> tex;
	XY pos{}, offset{}, texSize{}, flyTargetPos{};
	float baseY{};
	xx::List<XY> fixedPoss, fixedPosPool;
	xx::List<std::pair<int32_t, XY>> rocks;

	static constexpr float cRockRadius{ 64 };
	float cRocksScale{}, cCartBaseY{}, cCartBaseYLimit{};
	XY cRockMargin{}, cRockMarginOffsetRange{};
	int32_t cRocksMaxCount{};

	void Add(int32_t typeId_);
	void Init(Scene* scene_, XY pos_);
	void Draw() override;
};
