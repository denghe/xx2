#pragma once
#include "game.h"

namespace Test7 {

	enum class AnimTypes {
		Idle, Move, Atk
	};

	struct Scene;
	struct OrderByYItem {
		Scene* scene{};
		float y{};
		virtual void Update() {}
		virtual void Draw() {}
		~OrderByYItem() {}
	};

	struct Rock : OrderByYItem {
		XY pos{};
		float radius{};
		int32_t hp{};
		float whiteEndTime{};
		XY scale{};
		bool bouncing{};
		int32_t _2{};
		void Hit();
		void BeginWhite();
		void BeginBounce();
		void Bounce();	// _2
		Rock& Init(Scene* scene_, XY pos_, float radius_);
		void Update() override;
		void Draw() override;
	};

	struct Miner : OrderByYItem {
		xx::Frame* tfs{};
		char* cds{};
		int32_t minerTypeId{};
		int32_t tfsLen{};
		float tfIndex{};
		float resRadius{};
		float frameDelay{};
		float speedScale{};
		float radius{};
		bool flipX{};
		XY pos{};

		xx::Weak<Rock> target;
		XY targetPos{};
		float stepTime{};
		float attackRange{}, moveSpeed{};
		char hited{};
		int32_t _1{};

		Miner& Init(Scene* scene_, int32_t minerTypeId_, XY pos_, float radius_);
		void SetAnim(AnimTypes t);
		bool StepAnimOnce();
		void StepAnimLoop();
		bool SearchTarget();
		char GetHitData() const;
		void Update() override;
		void Draw() override;
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		float timer{};

		xx::List<xx::Shared<Miner>> miners;
		xx::List<xx::Shared<Rock>> rocks;
		xx::List<std::pair<float, OrderByYItem*>> obyis;	// for draw order

		void Init(int32_t minerTypeId_);
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
