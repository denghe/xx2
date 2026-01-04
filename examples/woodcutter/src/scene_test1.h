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
		_phys::Circle const* collisionData{};
		xx::Frame* frames{};
		int32_t framesLen{};
		int32_t collisionDataLen{};
		float frameIndex{};
		float frameInc{};
		int32_t treeTypeId{};

		// todo: store last hit info avoid hit every frame

		char state{};	// 0: idle  1: turn left   2: turn right
		char needWhite{};
		int32_t _1{}, _2{};
		float colorplus{};

		void Idle(bool fromTurn_ = false);
		void Turn(bool isLeft_);
		void White();
		bool Hit();	// return true: dead

		void _Idle();
		void _TurnLeft();
		void _TurnRight();
		void _White();

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
		bool lastMouseDirection{};	// false: left

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
