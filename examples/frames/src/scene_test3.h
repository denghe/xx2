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
		// for draw black border
		static constexpr float cBorderWidth{ 5 };
		// (gg.tp2.pics_circle64.uvRect.h + cBorderWidth * 2) / gg.tp2.pics_circle64.uvRect.h;
		static constexpr float cBlackBorderScale_Body{ 1.15625000 };
		// (gg.tp2.pics_barrel32.uvRect.h + cBorderWidth * 2) / gg.tp2.pics_barrel32.uvRect.h;
		static constexpr float cBlackBorderScale_Barrel{ 1.31250000 };

		static constexpr float cBarrelMinDistance{ 20 };
		static constexpr float cBarrelMaxDistance{ 48 };
		static constexpr float cBarrelMoveDistance{ cBarrelMaxDistance - cBarrelMinDistance };
		static constexpr float cShootDelay{ 0.5 };
		static constexpr float cShootStartupDuration{ 0.25 };
		static constexpr float cShootRecoveryDuration{ 0.03 };
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
		// for draw black border
		static constexpr float cBorderWidth{ 5 };
		// (gg.tp2.pics_circle32.uvRect.h + cBorderWidth * 2) / gg.tp2.pics_circle32.uvRect.h;
		static constexpr float cBlackBorderScale_Body{ 1.3125};
		static constexpr float cMaxLife{ 5 };
		static constexpr float cMoveFrameDistance{ 300 * gg.cDelta };

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
