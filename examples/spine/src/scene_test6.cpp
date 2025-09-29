#include "pch.h"
#include "scene_test6.h"
#include "scene_mainmenu.h"

void Eye::Init(Scene_Test6* scene_) {
	scene = scene_;
	sp.Emplace(gg.res.eye.skel);
	control = sp->FindBone("control");
	controlOriginal = { control->getX(), control->getY() };

	// right eye
	auto a = (spine::RegionAttachment*)sp->FindSlot("3")->getAttachment();
	eyeOriginalOffset = { /*a->getX()*/0, a->getY() };
}

void Eye::Update() {
	control->setX(controlOriginal.x + gg.mousePos.x - eyeOriginalOffset.x);
	control->setY(controlOriginal.y + gg.mousePos.y - eyeOriginalOffset.y);
	control->setAppliedValid(false);
	sp->Update(gg.delta);
}

void Eye::Draw() {
	sp->Draw(scene->cam.scale);
}



void Scene_Test6::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);
	eye.Init(this);
}

void Scene_Test6::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}
	eye.Update();
}

void Scene_Test6::Draw() {
	eye.Draw();
	gg.DrawNode(ui);
}

void Scene_Test6::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
