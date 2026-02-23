#pragma once
#include "game.h"

// simulate isaac game scene

namespace Test11 {
	static constexpr float cUIScale{ 0.5f };

	static constexpr float cCellPixelSize{ 128 };
	static constexpr float cCellPixelHalfSize{ cCellPixelSize / 2.f };
	static constexpr float cWallRadius{ cCellPixelSize / 2.f };
	static constexpr float cBucketRadius{ cWallRadius * 0.7f };
	static constexpr float cPlayerRadius{ cWallRadius * 0.7f };

	static constexpr XY cRoomMaxContentSize{ 26, 14 };	// logic
	static constexpr XY cRoomMaxTotalSize{ cRoomMaxContentSize + 2 };
	static constexpr XY cRoomMaxPixelSize{ cRoomMaxTotalSize * cCellPixelSize };

	static constexpr XY cRoom1x1ContentSize{ 13, 7 };
	static constexpr XY cRoom1x1TotalSize{ cRoom1x1ContentSize + 2 };
	static constexpr XY cRoom1x1PixelSize{ cRoom1x1TotalSize * cCellPixelSize };

	// ...

	struct Scene;

	struct SceneItem {
		static constexpr int32_t cTypeId{};
		Scene* scene{};
		SceneItem* next{};
		int32_t typeId{};	// fill by Init: typeId = cTypeId
		XY pos{};
		float y{};
		float scale{}, radians{}, radius{};
		bool flipX{}, isCenter{};
		int32_t indexAtContainer{ -1 };
		int32_t indexAtGrid{ -1 };
		virtual bool Update() { return false; }
		virtual void Draw() {};
		virtual void Dispose() {};	// unsafe: container.swapRemove( this )
		virtual ~SceneItem() {};
	};

	// todo: Player, Wall, Door, Rock, Bucket, Bomb....

	struct Wall : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_);
		void Draw() override;
		void Dispose() override;	// unsafe
		~Wall() override;
	};

	struct Door : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_, bool isCenter_);
		void Draw() override;
		void Dispose() override;	// unsafe
		~Door() override;
	};

	struct Player : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		static constexpr XY cSpeedMax{ 500.f / gg.cFps };
		static constexpr XY cAccel{ cSpeedMax / (gg.cFps * 0.1f) };
		XY lastMoveDir{}, speed{};
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;	// unsafe
		~Player() override;
	};

	struct Bucket : SceneItem {
		static constexpr int32_t cTypeId{ __LINE__ };
		void Init(Scene* scene_, XY pos_);
		bool Update() override;
		void Draw() override;
		void Dispose() override;	// unsafe
		~Bucket() override;
	};


	struct GridCache {
		XY pos{};
		float radius{};
		void operator=(SceneItem* p);
	};

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;
		float time{}, timePool{}, timeScale{ 1 };

		XY mapSize{};
		xx::Grid2dCircle<SceneItem*, GridCache> gridBuildings;	// for factory & wall
		xx::Grid2dCircle<SceneItem*, GridCache> gridItems;	// for scene items
		xx::List<xx::Shared<Wall>> walls;
		xx::List<xx::Shared<Door>> doors;
		xx::List<xx::Shared<Bucket>> buckets;
		xx::List<xx::Shared<Player>> players;

		void GenWallHorizontal(int32_t xFrom_, int32_t xTo_, int32_t y_, bool leftOverflow_ = false, bool rightOverflow_ = false);
		void GenWallVertical(int32_t x_, int32_t yFrom_, int32_t yTo_, bool topOverflow_ = false, bool bottomOverflow_ = false);
		void GenDoorHorizontal(int32_t x_, int32_t y_);
		void GenDoorVertical(int32_t x_, int32_t y_);
		void GenPlayer(int32_t x_, int32_t y_);

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
