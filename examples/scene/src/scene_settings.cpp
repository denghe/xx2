#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_settings.h"

void Scene_Settings::Init() {
	// init ui
	ui.Emplace()->InitRoot(gg.scale);

	// todo: drop down list
	// todo: checkbox slider

	// title
	// display mode
	// full screen( no border ) / window
	// music / sound volume
	// back:[esc]

	ui->MakeChildren<xx::Label>()->Init(2, gg.p5 + XY{ 0, 300 }, gg.a5, 5)
		.SetText("settings");

	ui->MakeChildren<xx::LabelButton>()->Init(2, gg.p5 + XY{ 0, 100 }, gg.a5
		, gg.cfg.s9bN, gg.cfg.s9bH
		, "asdf").onClicked = [this] {
			xx::CoutN("asdf");
			auto contentArea = ui->MakeChildren<xx::Scale9>();
			contentArea->Init(1, 0, 0.5, 1, { 300,300 }, *gg.cfg.s9bg);
			auto bg = ui->MakeChildren<xx::Background>();
			bg->Init(3, contentArea.ToWeak()).onOutsideClicked = [w = bg.ToWeak()] {
				w->contentArea->SwapRemoveFromParent();
				w->SwapRemoveFromParent();
				};
		};

	auto ddl = ui->MakeChildren<xx::DropDownList>();
	ddl->InitBegin(2, gg.p5 + XY{ 0, 0 }, gg.a5
		, { 500, 80 }, gg.cfg.s9bN, gg.cfg.s9bH, gg.cfg.s9bg);
	ddl->data.Add("1280x720", "1366x768", "1920x1080", "2560x1440", "3840x2160");
	ddl->InitEnd(2, gg.res.ui_dropdownlist_icon, gg.res.ui_dropdownlist_head);
}

void Scene_Settings::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}
}

void Scene_Settings::Draw() {
	gg.DrawNode(ui);
}

void Scene_Settings::OnResize() {
	ui->Resize(gg.scale);
}
