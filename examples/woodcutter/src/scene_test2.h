#pragma once
#include "game.h"
#include "particle.h"

namespace Test2 {

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::Particle p;

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
