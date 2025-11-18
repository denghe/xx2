#pragma once
#include "game.h"

namespace Test2 {
	struct Scene;
	struct Rock;

	// effect
	struct Pickaxe {
		XY pos{};
		float radians{};
		int32_t _1{};
		void Init(Rock* target_);
		bool Update();	// return true mean finished
		void Draw(Scene* scene_);
	};

	struct Rock {
		Scene* scene{};
		xx::TinyFrame tf;
		XY pos{}, centerPos{}, fixedPos{};
		int32_t indexAtGrid{ -1 };
		int32_t indexAtList{ -1 };	// fill by maker
		XY scale{};
		int32_t typeId{ -1 }, qualityId{ -1 };
		static constexpr float cHPMax{ 100 };
		int32_t hp{};
		int32_t _1{}, _2{};
		float breakFrameIndex{};
		bool flip{};
		bool ready{};	// true: can dig
		bool digging{};
		bool bouncing{};
		bool breaking{};
		Pickaxe pickaxe;	// digging coroutine
		void BeginDig();
		void BeginBounce();
		void Bounce();	// coroutine _2
		void Init(Scene* scene_);
		void Update();	// coroutine _1
		void Draw();
		void Dispose();
		~Rock();
	};

	struct FlyingRock {
		static constexpr float cFlySpeed{ 1200 / gg.cFps };
		static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.6f) };
		Scene* scene{};
		XY pos{}, inc{};
		float scale{}, scaleStep{}, moveCount{}, moveIndex{};
		int32_t typeId{};
		int32_t _1{};
		void Init(Rock* rock_);
		bool Update();	// coroutine _1
		void Draw(Scene* scene_);
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		static constexpr float cRockRadius{ 64 };
		XY cRockMargin{};
		XY cRockMarginOffsetRange{};
		int32_t cRocksMaxCount{};
		float cRocksScale{};
		float cMouseCircleRadius{};
		XY cRocksPivotOffset{};
		int32_t rocksDisposedCountPerFrame{};

		xx::RGBA8 cBGColor{};
		float cBGColorplus{};
		float cBGTiling{};

		xx::List<XY> rocksFixedPosPool;			// life cycle: must upon rocks
		xx::Grid2dCircle<Rock*> rocksGrid;		// life cycle: must upon rocks
		xx::List<xx::Shared<Rock>> rocks;
		xx::Shared<xx::GLTexture> texBG;
		std::array<XY, 9> flyTargets{};
		std::array<int32_t, 9> counts{};
		std::array<xx::Weak<xx::ImageLabelButton>, 9> countUIs;
		xx::List<FlyingRock> flyingRocks;

		void GenRocks(int32_t count_);
		void SortRocks();

		void MakeUI();
		void Init(float totalScale_ = 1);
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
