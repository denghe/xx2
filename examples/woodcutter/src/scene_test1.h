#pragma once
#include "game.h"

namespace Test1 {

	struct Scene;

	struct SceneItem {
		static constexpr int32_t cTypeId{ /* __LINE__ */ };
		Scene* scene{};
		SceneItem* next{};
		int32_t typeId{};
		float y{};
		XY pos{};
		virtual bool Update() { return {}; }
		virtual void Draw() {}
		virtual ~SceneItem() {}
	};

	struct Tree : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		xx::Frame* frames{};
		int32_t framesLen{};
		float frameIndex{};
		float frameInc{};
		int32_t treeTypeId{};

		int32_t state{};	// 0: idle  1: turn left   2: turn right
		int32_t _1{};

		void Idle();
		void TurnLeft();
		void TurnRight();

		void _Idle();
		void _TurnLeft();
		void _TurnRight();

		void Init(Scene* scene_, int32_t treeTypeId_, XY pos_);
		bool Update() override;
		void Draw() override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{}, mapCenterPos{};
		xx::List<XY> fixedPosPool;
		xx::List<xx::Shared<Tree>> trees;

		void MakeUI();
		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;

		/***********************************************/
		// for draw order by Y
		xx::List<SceneItem*> sortContainer;
		void SortContainerAdd(SceneItem* o);
		void SortContainerDraw();
		/***********************************************/
	};

}
