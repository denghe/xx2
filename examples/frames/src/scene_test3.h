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
		static constexpr float cBodyHeight{ gg.tp2._data_pics_circle64_png.uvRect.h };
		static constexpr float cBarrelHeight{ gg.tp2._data_pics_barrel32_png.uvRect.h };

		// for draw black border
		static constexpr float cBorderWidth{ 5 };
		static constexpr float cBorderScale_Body{ (cBodyHeight + cBorderWidth * 2) / cBodyHeight };
		static constexpr float cBorderScale_Barrel{ (cBarrelHeight + cBorderWidth * 2) / cBarrelHeight };

		static constexpr float cBarrelMinDistance{ 20 };
		static constexpr float cBarrelMaxDistance{ 40 };
		static constexpr float cBarrelMoveDistance{ cBarrelMaxDistance - cBarrelMinDistance };

		static constexpr float cShootDelay{ 1 };
		static constexpr float cShootStartupDuration{ 0.04 };
		static constexpr float cShootStunDuration{ 0.1 };
		static constexpr float cShootRecoveryDuration{ 0.4 };
		static constexpr float cBarrelStartupMoveStep{ cBarrelMoveDistance / (gg.cFps * cShootStartupDuration) };
		static constexpr int32_t cBarrelcShootStunCount{ int32_t(gg.cFps * cShootStunDuration) };
		static constexpr float cBarrelRecoveryMoveStep{ cBarrelMoveDistance / (gg.cFps * cShootRecoveryDuration) };

		XY pos{}, barrelPos{};
		bool shooting{};
		float shootDelay{ cShootDelay };
		float radians{};
		float nextShootTime{};
		float barrelDistance{ cBarrelMaxDistance };
		float barrelMinDistance{ cBarrelMinDistance };
		int32_t _1{}, _1i{};
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
		static constexpr float cBodyHeight{ gg.tp2._data_pics_circle32_png.uvRect.h };

		// for draw black border
		static constexpr float cBorderWidth{ 5 };
		static constexpr float cBorderScale_Body{ (cBodyHeight + cBorderWidth * 2) / cBodyHeight };

		static constexpr xx::RGB8 cColor{ xx::RGB8_White };
		static constexpr float cMaxLife{ 2 };
		static constexpr float cSpeed{ 120 };
		static constexpr float cMoveFrameDistance{ cSpeed * gg.cDelta };
		static constexpr float cAlphaStep{ 1. / (0.7 * gg.cFps) };
		static constexpr float cDampingMul{ 0.98f };

		int32_t _1{};
		XY pos{}, inc{};
		float radians{};
		float deadTime{};
		float alpha{};
		xx::RGBA8 color{};

		Scene* scene{};
		xx::Weak<Cannon> shooter;
		void Init(Cannon* shooter_);
		void Step();
		int32_t Update();
		void Draw1();
		void Draw2();
	};

}
