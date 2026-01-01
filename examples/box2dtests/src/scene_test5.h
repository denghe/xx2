#pragma once
#include "game.h"

namespace Test5 {
	static constexpr float cUIScale{ 0.5f };

	struct Scene;

	struct SceneItem {
		static constexpr int32_t cTypeId{};
		Scene* scene{};
		SceneItem* next{};
		int32_t typeId{};	// fill by Init: typeId = cTypeId
		XY pos{};
		float y{};
		float scale{}, radians{}, radius{};
		int32_t indexAtContainer{ -1 };
		int32_t indexAtGrid{ -1 };
		virtual bool Update() { return false; }
		virtual void Draw() {};
		virtual void Dispose() {};	// unsafe: container.swapRemove( this )
		virtual ~SceneItem() {};
	};

	struct DroppingItem : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		static constexpr float cSpeed{ 600.f / gg.cFps };
		xx::Frame frame;
		xx::FromTo<XY> dropPos{};
		XY inc{};
		int32_t numSteps{}, step{};
		float maxYOffset{}, yOffset{}, scale2{};
		void Init(Scene* scene_, float radius_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{};
		XY dropFrom{}, dropTo{};
		float dropFromRangeRadius{}, dropToRangeRadius{};
		float genSpeed{}, genPool{};
		xx::List<xx::Shared<DroppingItem>> droppingItems;

		xx::List<SceneItem*> sortContainer;			// for draw order by Y
		void SortContainerAdd(SceneItem* o_);
		void SortContainerDraw();

		void Init();
		void Update() override;
		virtual void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

}
