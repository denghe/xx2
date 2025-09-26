#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Scene_Test1::Init() {
	// init game logic
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);
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
}

void Scene_Test1::Draw() {
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
