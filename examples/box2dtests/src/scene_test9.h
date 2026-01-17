#pragma once
#include "game.h"

namespace Test9 {

	// todo

	struct Scene;
	struct SceneItem {
		Scene* scene{};
		xx::Frame frame;
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		XY pos{};	// center
		float y{}, radians{};
		virtual bool Update();
		virtual void Draw();
	};

	struct Block : SceneItem {
		XY size{};
		void Init(Scene* scene_, XY pos_, XY size_);
		void Draw() override;
	};

	struct Ball : SceneItem {
		float radius{};
		void Init(Scene* scene_, XY pos_, float radius_);
		void Draw() override;
	};

	struct Rock : SceneItem {
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		xx::List<Block> blocks;
		xx::List<Ball> balls;
		xx::List<xx::Shared<Rock>> rocks;

		float genTimer{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
