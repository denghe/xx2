#pragma once
#include "game.h"

namespace Test3 {

	struct Cannon;
	struct Bullet;

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		xx::Camera cam;

		float time{}, timePool{}, timeScale{ 1 };

		xx::Shared<Cannon> cannon;
		xx::List<xx::Shared<Bullet>> bullets;

		void Init();
		void Update() override;
		void FixedUpdate();
		void Draw() override;
		void OnResize(bool modeChanged_) override;
	};

	/*****************************************************************************************/
	/*****************************************************************************************/

	struct Cannon {
		// constexpr prepare
		static constexpr float cBodyHeight{ gg.tp2._data_pics_circle64.uvRect.h };
		static constexpr float cBarrelHeight{ gg.tp2._data_pics_barrel32.uvRect.h };

		// for draw black border
		static constexpr float cBorderWidth{ 5 };
		static constexpr float cBorderScale_Body{ (cBodyHeight + cBorderWidth * 2) / cBodyHeight };
		static constexpr float cBorderScale_Barrel{ (cBarrelHeight + cBorderWidth * 2) / cBarrelHeight };

		static constexpr float cBarrelMinDistance{ 20 };
		static constexpr float cBarrelMaxDistance{ 48 };
		static constexpr float cBarrelMoveDistance{ cBarrelMaxDistance - cBarrelMinDistance };
		static constexpr float cShootDelay{ 0.5 };
		static constexpr float cShootStartupDuration{ 0.25 };
		static constexpr float cShootRecoveryDuration{ 0.07 };
		static constexpr float cBarrelStartupMoveStep{ cBarrelMoveDistance / (gg.cFps * cShootStartupDuration) };
		static constexpr float cBarrelRecoveryMoveStep{ cBarrelMoveDistance / (gg.cFps * cShootRecoveryDuration) };

		XY pos{}, barrelPos{};
		bool shooting{};
		float radians{};
		float nextShootTime{};
		float barrelDistance{ cBarrelMinDistance };
		float barrelMinDistance{ cBarrelMinDistance };
		int32_t _1{};
		void Shoot();
		void ShootStep();
		void MakeBullet();

		Scene* scene{};
		void Init(Scene* scene_);
		void Update();
		void Draw1();
		void Draw2();
	};

	/*****************************************************************************************/
	/*****************************************************************************************/

	struct Bullet {
		// constexpr prepare
		static constexpr float cBodyHeight{ gg.tp2._data_pics_circle32.uvRect.h };

		// for draw black border
		static constexpr float cBorderWidth{ 5 };
		static constexpr float cBorderScale_Body{ (cBodyHeight + cBorderWidth * 2) / cBodyHeight };
		static constexpr float cMaxLife{ 5 };
		static constexpr float cMoveFrameDistance{ 30 * gg.cDelta };

		XY pos{}, inc{};
		float radians{};
		float deadTime{};

		Scene* scene{};
		xx::Weak<Cannon> shooter;
		void Init(Cannon* shooter_);
		int32_t Update();
		void Draw1();
		void Draw2();
	};

}
