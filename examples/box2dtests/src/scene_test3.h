#pragma once
#include "game.h"

namespace Test3 {

	struct Scene;

	struct SceneItem1 {
		Scene* scene{};
		xx::Frame frame;
		xx::B2Body b2body;
		int32_t indexAtContainer{-1};
		float scale{};
		bool isDead{};
		float Init(Scene* scene_, XY pos_, float scale_ = 1);
		bool Update();
		void Draw();
		void Dispose();	// unsafe
	};

	struct SceneItem2 {
		Scene* scene{};
		float radius{};
		XY pos{};
		void Init(Scene* scene_, XY pos_, float radius_);
		bool Update();
		void Draw();
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::List<std::pair<xx::Frame*, _phys::InitFunc>> frameAndFuncs;

		xx::B2World b2world;
		xx::List<xx::Shared<SceneItem1>> item1s;
		SceneItem2 item2;	// mouse
		xx::List<SceneItem1*> tmp;	// for delete

		float genTimer{};
		float lastGenY{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
