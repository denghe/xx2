#include "pch.h"
#include "scene_test5.h"
#include "scene_mainmenu.h"

void Man1::Init(Scene_Test5* scene_) {
	scene = scene_;
	sp.Emplace(gg.res.man1.skel);
	neck = sp->FindBone("neck");
	body = sp->FindBone("body");
	sp->SetAnimation(0, gg.res.man1.idle, true);
	neckOriginalPos = { neck->getX(), neck->getY() };
	neckOffset = {};
}

void Man1::Update() {
	sp->Update(gg.delta * 4);

	XY bodyOldPos{ body->getX(), body->getY() };
	XY bodyNewPos{};
	body->getParent()->worldToLocal(gg.mousePos.x * scene->cam._1_scale
		, gg.mousePos.y * scene->cam._1_scale, bodyNewPos.x, bodyNewPos.y);
	body->setX(bodyNewPos.x);
	body->setY(bodyNewPos.y);
	body->setAppliedValid(false);

	neckOffset -= bodyNewPos - bodyOldPos;
	// todo: max value protect?

	auto mag2 = neckOffset.x * neckOffset.x + neckOffset.y * neckOffset.y;
	auto step = neckFollowSpeed * gg.delta;
	if (mag2 < step * step) {
		neckOffset = {};
	} else if (mag2 > 0) {
		auto norm = neckOffset / std::sqrtf(mag2);
		neckOffset -= norm * step;
	}
	neck->setX(neckOriginalPos.x + neckOffset.x);
	neck->setY(neckOriginalPos.y + neckOffset.y);
	neck->setAppliedValid(false);

	sp->skeleton.updateWorldTransform();
}

void Man1::Draw() {
	sp->Draw();
}



void Scene_Test5::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);
	man1.Init(this);
}

void Scene_Test5::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}
	man1.Update();
}

void Scene_Test5::Draw() {
	man1.Draw();
	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test5::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
