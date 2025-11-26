#include "pch.h"
#include "scene_testa.h"
#include "scene_mainmenu.h"

namespace TestA {



/***************************************************************************************/

void Scene::Init() {
	cam.Init(gg.scale, 1.f, gg.designSize / 2);
	ui.Emplace()->InitRoot(gg.scale);

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
	// todo: add new craftrock
}

void Scene::Draw() {

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}

}
