#pragma once
#include "game.h"

// todo: priority queue sort by y
// todo: list free pos

struct Rock {
	xx::TinyFrame tf;
	XY pos{}, fixedPos{};
};

namespace std {
	template<> struct greater<xx::Shared<Rock>> {
		bool operator()(auto const& a, auto const& b) const {
			return a->pos.y > b->pos.y;
		}
	};
}

struct Scene_Test1 : xx::SceneBase {
	xx::Shared<xx::Node> ui;
	xx::Camera cam;
	float time{}, timePool{}, timeScale{ 1 };

	xx::Grid2dCircle<Rock*> grid;		// life cycle: must upon rocks
	xx::List<xx::Shared<Rock>> rocks;
	xx::List<XY> fixedPosPool;
	float rocksScale{ 0.25f };

	void Init();
	void Update() override;
	void FixedUpdate();
	void Draw() override;
	void OnResize(bool modeChanged_) override;
};
