#include "pch.h"
#include "scene_test4.h"
#include "scene_mainmenu.h"

void Scene_Test4::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);

	static constexpr int32_t numRows{ 30 }, numCols{ 50 };
	XY step{ gg.designSize.x / numCols, gg.designSize.y / numRows };
	XY offset{ -gg.designSize / 2 };
	for (size_t i = 0; i < numRows; i++) {
		for (size_t j = 0; j < numCols; j++) {
			auto& sb = sbs.Emplace();
			auto& sp = sb.sp;
			sp.Emplace(gg.res.spineBoy.skel);
			sp->SetPosition({ step.x * j + offset.x, step.y * i + offset.y });
			sp->SetScale(0.05);

			sp->SetUsePremultipliedAlpha(true);
			sp->SetAnimation(0, gg.res.spineBoy.walk, true);
			sp->SetAnimation(1, gg.res.spineBoy.aim, true);

			sb.crosshair = sp->FindBone("crosshair");
		}
	}
}

void Scene_Test4::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	auto mp = gg.mousePos / cam.scale;
	float boneCoordsX{}, boneCoordsY{};
	for (auto& sb : sbs) {
		sb.sp->Update(gg.delta);
		sb.crosshair->getParent()->worldToLocal(mp.x, mp.y, boneCoordsX, boneCoordsY);
		sb.crosshair->setX(boneCoordsX);
		sb.crosshair->setY(boneCoordsY);
		sb.crosshair->setAppliedValid(false);
		sb.sp->skeleton.updateWorldTransform();
	}
}

void Scene_Test4::Draw() {
	for (auto& sb : sbs) {
		sb.sp->Draw(cam.scale);
	}
	gg.DrawNode(ui);
}

void Scene_Test4::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
