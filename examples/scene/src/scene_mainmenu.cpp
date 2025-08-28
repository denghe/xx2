#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_1.h"
#include "scene_settings.h"

void Scene_MainMenu::Init() {
	// init ui
	ui.Emplace()->InitRoot(gg.scale);

	// todo: bg

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 300 }, gg.a5, 5)
		.SetText("game title");

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, 0 }, gg.a5
		, gg.cfg.s9bN, gg.cfg.s9bH
		, "play").onClicked = [this] {
			gg.MakeScene<Scene_1>()->Init();
		};

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, -100 }, gg.a5
		, gg.cfg.s9bN, gg.cfg.s9bH
		, "settings").onClicked = [this] {
			gg.MakeScene<Scene_Settings>()->Init();
		};

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, -200 }, gg.a5
		, gg.cfg.s9bN, gg.cfg.s9bH
		, "quit").onClicked = [this] {
			gg.running = false;
		};
}

void Scene_MainMenu::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.running = false;
		return;
	}
}

void Scene_MainMenu::Draw() {
	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize() {
	ui->Resize(gg.scale);
}
