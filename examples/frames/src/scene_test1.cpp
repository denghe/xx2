#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Scene_Test1::Init() {
	ui.Emplace()->InitRoot(gg.scale);
	cam.Init(gg.scale, 1.f, 0);
}

void Scene_Test1::Update() {
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

void Scene_Test1::FixedUpdate() {
}

void Scene_Test1::Draw() {
	gg.Quad().DrawFrame(gg.tp1.pics_p64, cam.ToGLPos({ -300, 0 }), cam.scale * 5);
	gg.Quad().DrawFrame(gg.tp2.pics_p64, cam.ToGLPos({ 300, 0 }), cam.scale * 5);
	gg.Quad().DrawFrame(gg.tp1.pics_a64, cam.ToGLPos({ -300, 0 }), cam.scale * 5, time);

	// ...
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
