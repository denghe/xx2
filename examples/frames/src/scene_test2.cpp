#include "pch.h"
#include "scene_test2.h"
#include "scene_mainmenu.h"

void Scene_Test2::Init() {
	ui.Emplace()->InitRoot(gg.scale);
	cam.Init(gg.scale, 2.f, 0);

	// combine tp1 & tp2 by RectPacker

	// step1: copy frame from gg
	tp1p64 = gg.tp1.pics_p64;
	tp2p64 = gg.tp2.pics_p64;
	tp1a64 = gg.tp1.pics_a64;

	// step2: combine & replace vars
	xx::RectPackerEx rp;
	rp.fs.Emplace(&tp1p64);
	rp.fs.Emplace(&tp2p64);
	rp.fs.Emplace(&tp1a64);
	rp.AutoPack(128, 1);
}

void Scene_Test2::Update() {
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

void Scene_Test2::FixedUpdate() {
}

void Scene_Test2::Draw() {
	gg.Quad().DrawFrame(tp1p64, cam.ToGLPos({ -100, -150 }), cam.scale);
	gg.Quad().DrawFrame(tp2p64, cam.ToGLPos({ 100, -150 }), cam.scale);
	gg.Quad().DrawFrame(tp1a64, cam.ToGLPos({ -100, -150 }), cam.scale, time);

	auto& tex = *tp1p64.tex;
	auto p = cam.ToGLPos({ -tex.size.x / 2, 150 });
	gg.Quad().Draw(tex, tex, p, 0, cam.scale);
	gg.Line().DrawRect(p, tex.size * cam.scale);

	// ...
	gg.DrawNode(ui);
}

void Scene_Test2::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
