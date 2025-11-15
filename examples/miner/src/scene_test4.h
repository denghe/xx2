#pragma once
#include "game.h"

namespace Test4 {

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
		Rock& Init(Scene* scene_, XY pos_, float radius_);
		void Update() override;
		void Draw() override;
	};

	struct Monster0 : OrderByYItem {
		xx::TinyFrame* tfs{};
		//XY* aps{};
		xx::FromTo<XY>* cds{};
		int32_t tfsLen{};
		float tfIndex{};
		float speedScale{};
		float radius{};
		bool flipX{};
		XY pos{};
		Monster0& Monster1Init(Scene* scene_, XY pos_, float radius_, float speedScale_);	// need set anim
		void SetAnim(AnimTypes t);
		bool StepAnimOnce();
		void StepAnimLoop();
		bool IsHitFrame() const;
		void Update() override;
		void Draw() override;
	};

	struct Monster1 : Monster0 {
		xx::Weak<Rock> target;
		XY targetPos{};
		float stepTime{};
		float attackRange{}, moveSpeed{};
		bool hited{};
		int32_t _1{};
		Monster1& Monster2Init(Scene* scene_, XY pos_, float radius_);
		bool SearchTarget();
		void Update() override;
		void Draw() override;
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		float timer{};

		xx::List<xx::Shared<Monster0>> monsters;
		xx::List<xx::Shared<Rock>> rocks;
		xx::List<std::pair<float, OrderByYItem*>> obyis;	// for draw order

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
