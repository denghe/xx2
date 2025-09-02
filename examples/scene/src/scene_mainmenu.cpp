#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_1.h"
#include "scene_settings.h"

void Scene_MainMenu::Init() {
	// init ui
	ui.Emplace()->InitRoot(gg.scale);

	if (!gg.langSelected) {
		ui->MakeChildren<UILangChoosePanel>()->Init(100).onClose = [this](i18n::Languages v) {
			gg.langSelected = true;				// set flag
			gg.lang.Set(v);						// set lang
			this->Init();						// reload ui ( unsafe )
		};
		return;
	}

	// todo: bg

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 300 }, gg.a5, 5)(gg.lang(Strs::gameName));

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, 0 }, gg.a5)(gg.lang(Strs::play)).onClicked = [this] {
		gg.MakeScene<Scene_1>()->Init();
	};

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, -100 }, gg.a5)(gg.lang(Strs::settings)).onClicked = [this] {
		gg.MakeScene<Scene_Settings>()->Init();
	};

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, -200 }, gg.a5)(gg.lang(Strs::quit)).onClicked = [this] {
		gg.running = false;
	};

	// todo: version? lang icon?
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
