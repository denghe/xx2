#include "pch.h"
#include "scene_mainmenu.h"
#include "scene_shader_grass.h"
#include "scene_shader_radiate_golden_rays.h"
// ...

void Scene_MainMenu::Init() {
	MakeUI();
}

void Scene_MainMenu::MakeUI() {
	// init ui
	static constexpr float cLineHeight{ 100 };
	static constexpr float cItemHeight{ 80 };
	static constexpr float cMargin{ 20 };

	auto basePos = gg.p5;
	auto offset = basePos + XY{ 0, 250 };
	auto anchor = gg.a5;
	auto fontSize = cItemHeight - gg.embed.cfg_s9bN->paddings.TopBottom();
	auto titleText = "custom shaders";
	auto fontSize2 = fontSize * 2;

	ui.Emplace()->InitRoot(gg.scale);

	offset.y -= cLineHeight * 1.5;
	ui->Make<xx::LabelButton>()->Init(2, offset, anchor, fontSize)("Scene_Shader_Grass").onClicked = [this] {
		gg.MakeScene<Scene_Shader_Grass>()->Init();
	};
	offset.y -= cLineHeight * 1.5;
	ui->Make<xx::LabelButton>()->Init(2, offset, anchor, fontSize)("Scene_Shader_RadiateGoldenRays").onClicked = [this] {
		gg.MakeScene<Scene_Shader_RadiateGoldenRays>()->Init();
	};
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
