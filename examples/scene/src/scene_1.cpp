#include "pch.h"
#include "scene_1.h"
#include "scene_mainmenu.h"

void Scene_1::Init() {
	// init ui
	static constexpr float cLineHeight{ 100 };
	static constexpr float cItemHeight{ 80 };
	static constexpr float cMargin{ 20 };
	auto fontSize = cItemHeight - gg.defaultCfg.s9bN->paddings.TopBottom();
	ui.Emplace()->InitRoot(gg.scale);
	ui->MakeChildren<xx::Label>()->Init(2, gg.p1 + cMargin, gg.a1, fontSize)("ver 0.1");
	ui->MakeChildren<xx::Label>()->Init(2, gg.p3 + XY{ -cMargin, cMargin }, gg.a3, fontSize)(gg.lang(Strs::escBack));

	// init game logic
	cam.Init(gg.scale, 1.f);

	int ny = 80;
	auto r = gg.designSize.y / ny;
	auto r_2 = r * 0.5f;
	auto nx = int(gg.designSize.x / r);

	auto x1 = -nx * 0.5f * r;
	auto x2 = nx * 0.5f * r;
	auto y1 = -ny * 0.5f * r;
	auto y2 = ny * 0.5f * r;

	for (auto y = y1; y < y2; y += r) {
		for (auto x = x1; x < x2; x += r) {
			monsters.Emplace().Emplace<Monster>()->Init(this
				, gg.res.heart, { x + r_2, y + r_2 }, r);
		}
	}
	monsters.Emplace().Emplace<Monster>()->Init(this
		, gg.res.heart, { -600, 0 }, 128);
	monsters.Emplace().Emplace<Monster>()->Init(this
		, gg.res.heart, { 600, 0 }, 128);
}

void Scene_1::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	// update game logic
	auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene_1::FixedUpdate() {
	for (auto& m : monsters) {
		m->Update();
	}
}

void Scene_1::Draw() {
	for (auto& m : monsters) {
		m->Draw();
	}

	gg.DrawNode(ui);
}

void Scene_1::OnResize() {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
