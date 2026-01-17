#pragma once
#include "game.h"

namespace Test9 {

	// todo
	static constexpr float cUIScale{ 0.5f };
	static constexpr XY mapSize{ 1920, 1920 }, mapSize_2{ mapSize / 2 };
	static constexpr float edgeWidth{ 100.f }, edgeWidth_2{ edgeWidth / 2 };
	static constexpr XY blockSize{ 12, 150 }, blockSize_2{ blockSize / 2 };
	static constexpr int32_t numBlocks{ 12 };
	static constexpr float ballRadius{ 10.f };
	static constexpr int32_t numBallRows{ 14 }, numBallCols{ 16 };
	static constexpr float baseY{ 300.f };
	static constexpr xx::FromTo<float> spaceXRange{ edgeWidth, mapSize.x - edgeWidth };

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
