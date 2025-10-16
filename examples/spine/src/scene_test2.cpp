#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"

void Scene_Test2::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);

	static constexpr int32_t numRows{ 30 }, numCols{ 50 };
	XY step { gg.designSize.x / numCols, gg.designSize.y / numRows };
	for (size_t i = 0; i < numRows; i++) {
		for (size_t j = 0; j < numCols; j++) {
			auto& owl = owls.Emplace();
			auto& sp = owl.sp;
			sp.Emplace(gg.res.owl.skel);
			sp->SetPosition({ step.x * j - gg.worldMaxXY.x, step.y * i - gg.worldMaxXY.y });
			sp->SetScale(0.05);

			sp->SetUsePremultipliedAlpha(true);
			sp->SetAnimation(0, gg.res.owl.idle, true);
			sp->SetAnimation(1, gg.res.owl.blink, true);

			owl.left = sp->SetAnimation(2, gg.res.owl.left, true);
			owl.right = sp->SetAnimation(3, gg.res.owl.right, true);
			owl.up = sp->SetAnimation(4, gg.res.owl.up, true);
			owl.down = sp->SetAnimation(5, gg.res.owl.down, true);

			owl.left->setAlpha(0);
			owl.left->setMixBlend(spine::MixBlend_Add);
			owl.right->setAlpha(0);
			owl.right->setMixBlend(spine::MixBlend_Add);
			owl.up->setAlpha(0);
			owl.up->setMixBlend(spine::MixBlend_Add);
			owl.down->setAlpha(0);
			owl.down->setMixBlend(spine::MixBlend_Add);
		}
	}
}

void Scene_Test2::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	float x = gg.mousePos.x / gg.designSize.x;
	float y = gg.mousePos.y / gg.designSize.y;
	for (auto& owl : owls) {
		if (x < 0) {
			owl.left->setAlpha(0);
			owl.right->setAlpha(-x * 2);
		}
		else {
			owl.left->setAlpha(x * 2);
			owl.right->setAlpha(0);
		}

		if (x < 0) {
			owl.up->setAlpha(0);
			owl.down->setAlpha(-y * 2);
		}
		else {
			owl.up->setAlpha(y * 2);
			owl.down->setAlpha(0);
		}

		owl.sp->skeleton.setToSetupPose();
		owl.sp->Update(gg.delta);
	}
}

void Scene_Test2::Draw() {
	for (auto& owl : owls) {
		owl.sp->Draw();
	}
	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test2::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
