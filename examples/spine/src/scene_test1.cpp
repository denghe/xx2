#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Scene_Test1::Init() {
	cam.Init(gg.scale, 0.5f);
	ui.Emplace()->InitRoot(gg.scale);

	sp.Emplace(gg.res.owl.skel);
	sp->SetUsePremultipliedAlpha(true);
	sp->SetAnimation(0, gg.res.owl.idle, true);
	sp->SetAnimation(1, gg.res.owl.blink, true);

	left = sp->SetAnimation(2, gg.res.owl.left, true);
	right = sp->SetAnimation(3, gg.res.owl.right, true);
	up = sp->SetAnimation(4, gg.res.owl.up, true);
	down = sp->SetAnimation(5, gg.res.owl.down, true);

	left->setAlpha(0);
	left->setMixBlend(spine::MixBlend_Add);
	right->setAlpha(0);
	right->setMixBlend(spine::MixBlend_Add);
	up->setAlpha(0);
	up->setMixBlend(spine::MixBlend_Add);
	down->setAlpha(0);
	down->setMixBlend(spine::MixBlend_Add);
}

void Scene_Test1::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	// fixed update
	auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene_Test1::FixedUpdate() {
	float x = gg.mousePos.x / gg.designSize.x;
	if (x < 0) {
		left->setAlpha(0);
		right->setAlpha(-x * 2);
	}
	else {
		left->setAlpha(x * 2);
		right->setAlpha(0);
	}

	float y = gg.mousePos.y / gg.designSize.y;
	if (x < 0) {
		up->setAlpha(0);
		down->setAlpha(-y * 2);
	}
	else {
		up->setAlpha(y * 2);
		down->setAlpha(0);
	}

	sp->skeleton.setToSetupPose();
	sp->Update(gg.cDelta);
}

void Scene_Test1::Draw() {
	sp->Draw(cam.scale);
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
