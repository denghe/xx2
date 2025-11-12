#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_test1.h"
#include "scene_test2.h"
#include "scene_test3.h"
#include "scene_test4.h"

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

	L.Append(C->Make<xx::Label>()->Init(2, 0, 0, fontSize * 2)("miner tests"));
	L.EndLine();

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1(rocks)")).onClicked = [this] {
		gg.MakeScene<Scene_Test1>()->Init();
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test2(dig rocks 1)")).onClicked = [this] {
		gg.MakeScene<Scene_Test2>()->Init(1);
	};
	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test2(dig rocks 5)")).onClicked = [this] {
		gg.MakeScene<Scene_Test2>()->Init(5);
	};
	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test2(dig rocks 10)")).onClicked = [this] {
		gg.MakeScene<Scene_Test2>()->Init(10);
	};
	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test3(airplanes)")).onClicked = [this] {
		gg.MakeScene<Scene_Test3>()->Init();
	};
	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test4(monster anim)")).onClicked = [this] {
		gg.MakeScene<Scene_Test4>()->Init();
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
	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
}
