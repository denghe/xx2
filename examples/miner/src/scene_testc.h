#pragma once
#include "game.h"

namespace TestC {

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
		virtual ~SceneItem() {}
	};

	struct Miner : SceneItem {
		xx::Frame* frames{};
		int32_t framesLen{};
		int32_t frameIndex{};
		float yOffset{};		// for Draw: pos.y = y + yOffset
		float pcDiff{};			// fill by InitYOffset()
		XY moveTargetPos{};
		void Init(Scene* scene_, int32_t index_, XY pos_);
		int32_t _1{};
		int Update() override;
		void Draw() override;

		void InitYOffset();
		void UpdateYOffset();
		void SetMoveTargetPos();
		int32_t MoveToTarget();

		int32_t _2{};
		float bounceHeight{}, bounceInc{}, radians{}, radiansStep{}, radiansTarget{}, timer{};
		float bounceHeightMax{}, bounceHalfDuration{};	// call AnimBounceRotate() before: need fill
		bool bouncing{};
		void AnimBounceRotate();
		void AnimInit();		// init args
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{}, mapCenterPos{};
		xx::List<xx::Shared<Miner>> miners;

		void GenMiners(int32_t count_);

		/***********************************************/
		// for draw order by Y
		xx::List<SceneItem*> sortContainer;
		void SortContainerAdd(SceneItem* o);
		void SortContainerDraw();
		/***********************************************/

		void Init(float logicScale_, int32_t count_);
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
