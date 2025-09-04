#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_1.h"
#include "ui_settings.h"
#include "ui_settings_lang.h"

void Scene_MainMenu::Init() {
	bak = gg.lang.language;
	MakeUI();
	// first boot? popup lang select panel
	if (!gg.langSelected) {
		ui->MakeChildren<UI::Settings_Lang>()->Init(100);
		return;
	}
}

void Scene_MainMenu::MakeUI() {
	// init ui
	ui.Emplace()->InitRoot(gg.scale);

	// todo: bg

	auto fontSize = 80 - gg.defaultCfg.s9bN->paddings.TopBottom();

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 300 }, gg.a5, fontSize * 2)(gg.lang(Strs::gameName));

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, 0 }, gg.a5, fontSize)(gg.lang(Strs::play)).onClicked = [this] {
		gg.MakeScene<Scene_1>()->Init();
	};

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, -100 }, gg.a5, fontSize)(gg.lang(Strs::settings)).onClicked = [this] {
		ui->MakeChildren<UI::Settings>()->Init(100);
	};

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, -200 }, gg.a5, fontSize)(gg.lang(Strs::quit)).onClicked = [this] {
		gg.running = false;
	};

	ui->MakeChildren<xx::Label>()->Init(2, gg.p1 + XY{ 20, 20 }, gg.a1, fontSize)("ver 0.1");

	// todo:  lang icon?
}

void Scene_MainMenu::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		if (auto node = ui->FindTopESCHandler(); node) {
			node->HandleESC();
		}
		else {
			gg.running = false;
		}
		return;
	}

	if (bak != gg.lang.language) {
		gg.langSelected = true;
		Init();
	}
}

void Scene_MainMenu::Draw() {
	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize() {
	ui->Resize(gg.scale);
}
