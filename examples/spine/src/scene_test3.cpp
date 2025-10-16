#include "pch.h"
#include "scene_test3.h"
#include "scene_mainmenu.h"

void Scene_Test3::Init() {
	cam.Init(gg.scale, 0.5f);
	ui.Emplace()->InitRoot(gg.scale);

	sp.Emplace(gg.res.spineBoy.skel);
	//sp->SetScale(cam.scale);
	sp->SetPosition({ 0, -300 });
	sp->SetUsePremultipliedAlpha(true);
	sp->SetAnimation(0, gg.res.spineBoy.walk, true);
	sp->SetAnimation(1, gg.res.spineBoy.aim, true);
	crosshair = sp->FindBone("crosshair");
}

void Scene_Test3::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	if (gg.mouse[GLFW_MOUSE_BUTTON_1](0.2f)) {
		sp->SetAnimation(2, gg.res.spineBoy.shoot, false);
	}

	// todo: bug fix
	//if (gg.keyboard[GLFW_KEY_SPACE](0.2f)) {
	//	sp->SetAnimation(3, gg.res.spineBoy.jump, false);
	//}

	sp->Update(gg.delta);

	float boneCoordsX{}, boneCoordsY{};
	crosshair->getParent()->worldToLocal(gg.mousePos.x, gg.mousePos.y, boneCoordsX, boneCoordsY);
	crosshair->setX(boneCoordsX);
	crosshair->setY(boneCoordsY);
	crosshair->setAppliedValid(false);
	sp->skeleton.updateWorldTransform();
}

void Scene_Test3::Draw() {
	sp->Draw();
	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test3::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
