#pragma once
#include "game.h"

namespace Test9 {

	struct Scene;
	struct OrderByYItem {
		Scene* scene{};
		float y{};
		virtual void Update() {}
		virtual void Draw() {}
		~OrderByYItem() {}
	};

	struct CartRock : OrderByYItem {
		int32_t typeId{};
		XY pos{};
		CartRock& Init(Scene* scene_, XY pos_);
		void Draw() override;
	};

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		float timer{};

		static constexpr float cRockRadius{ 64 };
		float cRocksScale{};
		XY cRockMargin{}, cRockMarginOffsetRange{};
		int32_t cRocksMaxCount{};
		float cCartBaseY{};

		xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
		xx::List<xx::Shared<CartRock>> cartrocks;
		xx::List<std::pair<float, OrderByYItem*>> obyis;	// for draw order

		void AddCartRocks();

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
