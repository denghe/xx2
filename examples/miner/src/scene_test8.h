#pragma once
#include "game.h"

namespace Test8 {

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

	struct Scene : xx::SceneBase {
		static constexpr float cUIScale{ 0.5f };
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		float timer{};

		static constexpr float cRockRadius{ 64 };
		XY cRockMargin{};
		XY cRockMarginOffsetRange{};
		int32_t cRocksMaxCount{};
		float cRocksScale{};
		float cMouseCircleRadius{};
		XY cRocksPivotOffset{};
		int32_t rocksDisposedCountPerFrame{};

		xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
		xx::List<xx::Shared<Rock>> rocks;
		xx::List<std::pair<float, OrderByYItem*>> obyis;	// for draw order

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
