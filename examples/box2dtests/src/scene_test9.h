#pragma once
#include "game.h"

namespace Test9 {

	// todo
	static constexpr float cUIScale{ 0.5f };
	static constexpr XY mapSize{ 1920, 1920 }, mapSize_2{ mapSize / 2 };
	static constexpr float edgeWidth{ 100.f }, edgeWidth_2{ edgeWidth / 2 };
	static constexpr XY blockSize{ 12, 150 }, blockSize_2{ blockSize / 2 };
	static constexpr int32_t numBlocks{ 12 };
	static constexpr float ballRadius{ 10.f }, rockMinRadius{ 25.f };
	static constexpr int32_t numBallRows{ 14 }, numBallCols{ 16 };
	static constexpr float baseY{ 300.f };
	static constexpr xx::FromTo<float> spaceXRange{ edgeWidth + rockMinRadius, mapSize.x - edgeWidth - rockMinRadius };

	struct Scene;
	struct SceneItem {
		//static constexpr int32_t cTypeId{ __LINE__ };
		int32_t typeId{};	// sould be set in Init func
		float y{};
		Scene* scene{};
		xx::Frame frame;
		xx::B2Body b2body;
		xx::B2Shape b2shape;
		XY pos{};	// center
		float radians{}, radius{};
		virtual bool Update();
		virtual void Draw();
		virtual ~SceneItem() {};
	};

	struct Block : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		XY size{};
		void Init(Scene* scene_, XY pos_, XY size_);
		bool Update() override;
		void Draw() override;
	};

	struct Rock;
	struct Ball : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		xx::RGBA8 color{};
		bool bouncing{}, disabing{};
		int16_t sameRockHitCount{};
		int32_t _1{}, _1i;
		XY _1inc{}, offset{};
		float scale{}, colorplus{};
		Rock* lastRock{};
		void Bounce();
		void Hit(Rock* rock_);
		void Init(Scene* scene_, XY pos_, float radius_);
		bool Update() override;
		void Draw() override;
	};

	struct Rock : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		xx::B2World b2world;
		xx::List<xx::Shared<Block>> blocks;
		xx::List<xx::Shared<Ball>> balls;
		xx::List<xx::Shared<Rock>> rocks;
		xx::EffectTextManager effectTextManager;

		float genTimer{};
		void Gen(int32_t num_);

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
