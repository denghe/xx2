#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

namespace Test3 {

	void Scene::Init() {
		ui.Emplace()->InitRoot(gg.scale);
		cam.Init(gg.scale, 2.f, 0);

		gg.SetDefaultGLClearColor({255,0,0,255});
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

		if (cannon->shootDelay > 0.01f) {
			cannon->shootDelay -= 0.001f;
		}
	}

	void Scene::Draw() {
		//gg.Quad().DrawFrame(gg.tp2.pics_circle64_png, {}, 10.f * cam.scale);

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
		// reset coroutine
		_1 = 0;
		shooting = true;
		nextShootTime = gg.time + shootDelay;
		barrelDistance = cBarrelMaxDistance;
	}

	void Cannon::ShootStep() {
		XX_BEGIN(_1);

		// shoot
		MakeBullet();

		// startup
		while (barrelDistance > cBarrelMinDistance) {
			XX_YIELD(_1);
			barrelDistance -= cBarrelStartupMoveStep;
		}
		barrelDistance = cBarrelMinDistance;

		// stun
		for (_1i = 0; _1i < cBarrelcShootStunCount; ++_1i) {
			XX_YIELD(_1);
		}

		// recovery
		while (barrelDistance < cBarrelMaxDistance) {
			XX_YIELD(_1);
			barrelDistance += cBarrelRecoveryMoveStep;
		}
		barrelDistance = cBarrelMaxDistance;

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
		gg.Quad().DrawFrame(gg.tp2.pics_circle64_png, cam.ToGLPos(pos), cBorderScale_Body * cam.scale, -radians, 1, xx::RGBA8_Black);
		gg.Quad().DrawFrame(gg.tp2.pics_barrel32_png, cam.ToGLPos(barrelPos), cBorderScale_Barrel * cam.scale, -radians, 1, xx::RGBA8_Black);
	}

	void Cannon::Draw2() {
		auto& cam = scene->cam;
		// draw shape
		gg.Quad().DrawFrame(gg.tp2.pics_circle64_png, cam.ToGLPos(pos), cam.scale, -radians);
		gg.Quad().DrawFrame(gg.tp2.pics_barrel32_png, cam.ToGLPos(barrelPos), cam.scale, -radians);

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
		alpha = 1.f;
		color.SetPMAColor(cColor, alpha);
	}

	void Bullet::Step() {
		XX_BEGIN(_1);
		while (deadTime > gg.time) {
			XX_YIELD(_1);
			pos += inc;
		}
		while (alpha > 0) {
			XX_YIELD(_1);
			inc *= cDampingMul;
			pos += inc;
			color.SetPMAColor(cColor, ((alpha -= cAlphaStep)));
		}
		color.SetPMAColor(cColor, ((alpha = 0)));
		XX_END(_1);
	}

	int32_t Bullet::Update() {
		Step();
		return alpha == 0;
	}

	void Bullet::Draw1() {
		auto& cam = scene->cam;
		// draw black border
		gg.Quad().DrawFrame(gg.tp2.pics_circle32_png, cam.ToGLPos(pos)
			, cBorderScale_Body * cam.scale, -radians, 1, {0,0,0,color.a});
	}

	void Bullet::Draw2() {
		auto& cam = scene->cam;
		// draw shape
		gg.Quad().DrawFrame(gg.tp2.pics_circle32_png, cam.ToGLPos(pos)
			, cam.scale, -radians, 1, color);
	}

}
