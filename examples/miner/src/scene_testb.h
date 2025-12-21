#pragma once
#include "game.h"

namespace TestB {

	struct Scene;
	struct SceneItem {
		Scene* scene{};
		SceneItem* next{};
		float y{}, scale{};
		XY pos{};
		float colorPlus{};
		bool flipX{};
		virtual int Update() { return 0; }
		virtual void Draw() {}
		virtual void DrawLight() {}
		virtual ~SceneItem() {}
	};

	struct Rock : SceneItem {
		int32_t typeId{}, qualityId{};
		int32_t gridIndex{ -1 };
		void Init(Scene* scene_, int32_t typeId_, int32_t qualityId_, XY pos_);
		void Draw() override;
	};

	struct Grass : SceneItem {
		xx::Frame* frames{};
		int32_t framesLen{};
		float frameIndex{};
		int32_t gridIndex{ -1 };
		void Init(Scene* scene_, XY pos_, bool randomFrameIndex_ = true);
		int Update() override;
		void Draw() override;
	};

	struct Miner : SceneItem {
		xx::Frame* frames{};
		int32_t framesLen{};
		int32_t frameIndex{};
		void Init(Scene* scene_, int32_t index_, XY pos_);
		int Update() override;
		void Draw() override;
		void DrawLight() override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };
		xx::FrameBuffer fb;										// for light

		int32_t cGrassCount{};
		bool cGrassRandomFrameIndex{};
		float cLeafScale{};
		float cLeafColorplus{};
		int32_t cLeafCount{};
		xx::RGBA8 cBGColor{};
		float cBGColorplus{};
		float cBGTiling{};
		static constexpr float cGrassRadius{ 64 };
		XY cGrassMarginOffsetRange{};
		int32_t cGrassMaxCount{};
		xx::List<Grass> grasses;

		xx::List<XY> rocksFixedPosPool;
		xx::List<Rock> rocks;

		xx::List<xx::Shared<Miner>> miners;

		void GenGrass();
		void GenRocks();
		void GenMiners();

		/***********************************************/
		// for draw order by Y
		xx::List<SceneItem*> sortContainer;
		void SortContainerAdd(SceneItem* o);
		void SortContainerDraw();
		/***********************************************/

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
