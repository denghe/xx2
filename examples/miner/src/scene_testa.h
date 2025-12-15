#pragma once
#include "game.h"

namespace TestA {

	struct Scene;
	struct Miner {
		Scene* scene{};
		xx::Frame* frames{};
		int32_t framesLen{};
		int32_t frameIndex{};
		XY pos{};
		void Init(Scene* scene_, int32_t index_, XY pos_);
		void Update();
		void Draw();
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::List<xx::Shared<Miner>> miners;

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
