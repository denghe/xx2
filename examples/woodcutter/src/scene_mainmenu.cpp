#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_test1.h"
#include "scene_test2.h"

void Scene_MainMenu::Init() {
	MakeUI();
}

void Scene_MainMenu::MakeUI() {
	static constexpr float cLineHeight{ 100 };
	static constexpr float cItemHeight{ 80 };
	static constexpr float cMargin{ 20 };
	auto fontSize = cItemHeight - gg.embed.cfg_s9bN->paddings.TopBottom();

	ui.Emplace()->InitRoot(gg.scale);
	auto C = ui->Make<xx::Node>();
	xx::Layouter L;
	L.InitBegin(C, 2, gg.p7 + XY{ 30, 0 }, { 0, 1 }, gg.designSize.x - 60)
		.HAlign(xx::HAligns::Center)
		.LeftMargin(cMargin)
		.DefaultLineHeight(cLineHeight);

	L.Append(C->Make<xx::Label>()->Init(2, 0, 0, fontSize * 2)("woodcutter tests"));
	L.EndLine();

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 1 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.1, 4, 0.2);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 2x2 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.15, 2, 0.3);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 3x3 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.2, 1.2, 0.4);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 4x4 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.25, 1, 0.5);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 5x5 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.3, 0.8, 0.63);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 6x6 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.35, 0.7, 0.72);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 7x7 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.4, 0.6, 0.9);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 8x8 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.4, 0.5, 1);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( 9x9 )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.4, 0.4, 1.15);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( NxN )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init(1.4, 0.1, 5);
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test2")).onClicked = [this] {
		gg.MakeScene<Test2::Scene>()->Init();
	};


	L.InitEnd();
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
}

void Scene_MainMenu::Draw() {
#if 0
	auto&& t = *gg._pics.a_[0].tex;
	gg.Quad().Draw(t, t, 0, 0.5f, gg.designSize.y / t.size.y * gg.scale);
#endif

	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
}
