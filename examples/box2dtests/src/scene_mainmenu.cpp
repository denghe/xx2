#include "pch.h"
#include "g.h"
#include "scene_mainmenu.h"
#include "scene_test1.h"
#include "scene_test2.h"
#include "scene_test3.h"
#include "scene_test4.h"
#include "scene_test5.h"
#include "scene_test6.h"

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
	auto hds = gg.designSize * 0.5f;
	L.InitBegin(C, 2, {-hds.x + 30, hds.y }, { 0, 1 }, gg.designSize.x - 60)
		.HAlign(xx::HAligns::Center)
		.LeftMargin(cMargin)
		.DefaultLineHeight(cLineHeight);

	L.Append(C->Make<xx::Label>()->Init(2, 0, 0, fontSize * 2)("box2d v3 tests"));
	L.EndLine();

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test1( stars )")).onClicked = [this] {
		gg.MakeScene<Test1::Scene>()->Init();
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test2( fishs )")).onClicked = [this] {
		gg.MakeScene<Test2::Scene>()->Init();
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test3( cut wood )")).onClicked = [this] {
		gg.MakeScene<Test3::Scene>()->Init();
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test4( cut wood -> box )")).onClicked = [this] {
		gg.MakeScene<Test4::Scene>()->Init();
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test5( drop effect )")).onClicked = [this] {
		gg.MakeScene<Test5::Scene>()->Init();
	};

	L.Append(C->Make<xx::LabelButton>()->Init(2, 0, 0, fontSize)("test6( rocks drop into bag )")).onClicked = [this] {
		gg.MakeScene<Test6::Scene>()->Init();
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
	gg.SetBlendPremultipliedAlpha(false);
	gg.DrawNode(ui);
}

void Scene_MainMenu::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
}
