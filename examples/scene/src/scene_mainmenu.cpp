#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_play.h"
#include "ui_settings.h"
#include "ui_settings_lang.h"

void Scene_MainMenu::Init() {
	bak = gg.lang.language;
	MakeUI();
	// first boot? popup lang select panel
	if (!gg.settings.langSelected) {
		ui->Make<UI::Settings_Lang>()->Init(100);
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
	auto fontSize = cItemHeight - gg.embed.cfg_s9bN->paddings.TopBottom();

	ui.Emplace()->InitRoot(gg.scale);

	ui->Make<xx::Label>()->Init(2, offset, anchor, fontSize * 2)(gg.lang(Strs::gameName));

	offset.y -= cLineHeight * 1.5;

	auto& img = gg.embed.icon_flags_[(int32_t)gg.lang.language];
	ui->Make<xx::ImageButton>()->Init(2, offset, anchor, cItemHeight, 4, img).onClicked = [this] {
		ui->Make<UI::Settings_Lang>()->Init(100);
	};

	offset.y -= cLineHeight;
	ui->Make<xx::LabelButton>()->Init(2, offset, anchor, fontSize)(gg.lang(Strs::play)).onClicked = [this] {
		gg.MakeScene<Scene_Play>()->Init();
	};

	offset.y -= cLineHeight;
	ui->Make<xx::ImageLabelButton>()->Init(2, offset, anchor, fontSize)(gg.embed.icon_gear, cLineHeight, cItemHeight * 0.5f)(gg.lang(Strs::settings)).onClicked = [this] {
		ui->Make<UI::Settings>()->Init(100);
	};

	offset.y -= cLineHeight;
	ui->Make<xx::LabelButton>()->Init(2, offset, anchor, fontSize)(gg.lang(Strs::quit)).onClicked = [this] {
		gg.running = false;
	};

	ui->Make<xx::Label>()->Init(2, gg.p1 + cMargin, gg.a1, fontSize)("ver 0.1");

	ui->Make<xx::Label>()->Init(2, gg.p3 + XY{ -cMargin, cMargin }, gg.a3, fontSize)(gg.lang(Strs::escBack));
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
	// bg?
	gg.Quad().Draw(*gg.res.blade.tex, gg.res.blade.tex->Rect(), {-500, 0}, 0);
	gg.Quad().Draw(*gg.res.player.tex, gg.res.player.textureRect, {500, 0}, 0);

	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize() {
	ui->Resize(gg.scale);
}
