#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale);
		cam.Init(gg.scale, 2.f, 0);

		gg.SetDefaultGLClearColor(xx::RGBA8_Red);

		cannon.Emplace()->scene = this;
	}

	void Scene::Update() {
		// handle inputs
		if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
			gg.MakeScene<Scene_MainMenu>()->Init();
			return;
		}

		// fixed update
		auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
		timePool += d;
		while (timePool >= gg.cDelta) {
			time += gg.cDelta;
			timePool -= gg.cDelta;
			FixedUpdate();
		}
	}

	void Scene::FixedUpdate() {
		for (int32_t i = bullets.len - 1; i >= 0; --i) {
			if (bullets[i]->Update()) {
				bullets.SwapRemoveAt(i);
			}
		}
		cannon->Update();
	}

	void Scene::Draw() {
		// draw1
		cannon->Draw1();
		for (int32_t i = 0, e = bullets.len; i < e; ++i) {
			bullets[i]->Draw1();
		}
		// draw2
		cannon->Draw2();
		for (int32_t i = 0, e = bullets.len; i < e; ++i) {
			bullets[i]->Draw2();
		}

		// ...
		gg.DrawNode(ui);
	}

	void Scene::OnResize(bool modeChanged_) {
		ui->Resize(gg.scale);
		cam.SetBaseScale(gg.scale);
	}

	/*****************************************************************************************/
	/*****************************************************************************************/

	void Cannon::Init(Scene* scene_) {
		scene = scene_;
	}

	void Cannon::Shoot() {
		//assert(cShootStartupDuration + cShootRecoveryDuration <= cShootDelay);
		//barrelMinDistance = cBarrelMaxDistance - 
		shooting = true;
		nextShootTime = gg.time + cShootDelay;
		_1 = 0;
	}

	void Cannon::ShootStep() {
		XX_BEGIN(_1);

		// startup
		while (barrelDistance < cBarrelMaxDistance) {
			XX_YIELD(_1);
			barrelDistance += cBarrelStartupMoveStep;
		}
		barrelDistance = cBarrelMaxDistance;

		// shoot bullet
		MakeBullet();

		// recovery
		while (barrelDistance > cBarrelMinDistance) {
			XX_YIELD(_1);
			barrelDistance -= cBarrelRecoveryMoveStep;
		}
		barrelDistance = cBarrelMinDistance;

		// flag restore
		shooting = false;
		XX_END(_1);
	}

	void Cannon::MakeBullet() {
		scene->bullets.Emplace().Emplace()->Init(this);
	}

	void Cannon::Update() {
		// calculate barrel's radians
		auto d = scene->cam.ToLogicPos(gg.mousePos) - pos;
		radians = std::atan2(d.y, d.x);

		if (shooting) {
			ShootStep();
		}

		if (nextShootTime <= gg.time) {
			Shoot();
		}

		// calculate barrel's pos by radians
		barrelPos = pos + xx::RotatePoint({ barrelDistance, 0 }, radians);
	}

	void Cannon::Draw1() {
		auto& cam = scene->cam;
		// draw black border
		gg.Quad().DrawFrame(gg.tp2.pics_circle64, cam.ToGLPos(pos), cBorderScale_Body * cam.scale, -radians, 1, xx::RGBA8_Black);
		gg.Quad().DrawFrame(gg.tp2.pics_barrel32, cam.ToGLPos(barrelPos), cBorderScale_Barrel * cam.scale, -radians, 1, xx::RGBA8_Black);
	}

	void Cannon::Draw2() {
		auto& cam = scene->cam;
		// draw shape
		gg.Quad().DrawFrame(gg.tp2.pics_circle64, cam.ToGLPos(pos), cam.scale, -radians);
		gg.Quad().DrawFrame(gg.tp2.pics_barrel32, cam.ToGLPos(barrelPos), cam.scale, -radians);

	}

	/*****************************************************************************************/
	/*****************************************************************************************/


	void Bullet::Init(Cannon* shooter_) {
		scene = shooter_->scene;
		shooter = xx::WeakFromThis(shooter_);
		pos = shooter_->barrelPos;
		radians = shooter_->radians;
		inc.x = std::cosf(radians) * cMoveFrameDistance;
		inc.y = std::sinf(radians) * cMoveFrameDistance;
		deadTime = gg.time + cMaxLife;
	}

	int32_t Bullet::Update() {
		if (deadTime <= gg.time) return 1;
		pos += inc;
		return 0;
	}

	void Bullet::Draw1() {
		auto& cam = scene->cam;
		// draw black border
		gg.Quad().DrawFrame(gg.tp2.pics_circle32, cam.ToGLPos(pos), cBorderScale_Body  * cam.scale, -radians, 1, xx::RGBA8_Black);
	}

	void Bullet::Draw2() {
		auto& cam = scene->cam;
		// draw shape
		gg.Quad().DrawFrame(gg.tp2.pics_circle32, cam.ToGLPos(pos), cam.scale, -radians);
	}

}
