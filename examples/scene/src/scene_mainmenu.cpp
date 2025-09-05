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

/*
+-----------------------------+
		   title

           [lang]
		    play
		  settings
		    quit

ver					  [ESC]back
+-----------------------------+
*/

void Scene_MainMenu::MakeUI() {
	// init ui
	static constexpr float cLineHeight{ 100 };
	static constexpr float cItemHeight{ 80 };
	static constexpr float cMargin{ 20 };

	auto basePos = gg.p5;
	auto offset = basePos + XY{ 0, 250 };
	auto anchor = gg.a5;
	auto fontSize = cItemHeight - gg.defaultCfg.s9bN->paddings.TopBottom();

	ui.Emplace()->InitRoot(gg.scale);

	ui->MakeChildren<xx::Label>()->Init(2, offset, anchor, fontSize * 2)(gg.lang(Strs::gameName));

	offset.y -= cLineHeight * 1.5;

	auto& img = gg.res.ui_flags_[(int32_t)gg.lang.language];
	ui->MakeChildren<xx::ImageButton>()->Init(2, offset, anchor, cItemHeight, 4, img).onClicked = [this] {
		ui->MakeChildren<UI::Settings_Lang>()->Init(100);
	};

	offset.y -= cLineHeight;
	ui->MakeChildren<xx::LabelButton>()->Init(2, offset, anchor, fontSize)(gg.lang(Strs::play)).onClicked = [this] {
		gg.MakeScene<Scene_1>()->Init();
	};

	offset.y -= cLineHeight;
	ui->MakeChildren<xx::LabelButton>()->Init(2, offset, anchor, fontSize)(gg.lang(Strs::settings)).onClicked = [this] {
		ui->MakeChildren<UI::Settings>()->Init(100);
	};

	offset.y -= cLineHeight;
	ui->MakeChildren<xx::LabelButton>()->Init(2, offset, anchor, fontSize)(gg.lang(Strs::quit)).onClicked = [this] {
		gg.running = false;
	};

	ui->MakeChildren<xx::Label>()->Init(2, gg.p1 + cMargin, gg.a1, fontSize)("ver 0.1");

	ui->MakeChildren<xx::Label>()->Init(2, gg.p3 + XY{ -cMargin, cMargin }, gg.a3, fontSize)(gg.lang(Strs::escBack));
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
		Init();
	}
}

void Scene_MainMenu::Draw() {
	// todo: bg?
	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize() {
	ui->Resize(gg.scale);
}
