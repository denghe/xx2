#include "pch.h"
#include "scene_play.h"
#include "scene_mainmenu.h"

// todo: equip + bag + props list + log panel
// game logic: mouse is knife, hit circle monster

void Scene_Play::Init() {
	// init ui
	static constexpr float cLineHeight{ 100 };
	static constexpr float cItemHeight{ 80 };
	static constexpr float cMargin{ 20 };
	auto fontSize = cItemHeight - gg.embed.cfg_s9bN->paddings.TopBottom();
	ui.Emplace()->InitRoot(gg.scale);
	ui->Make<xx::Label>()->Init(2, gg.p1 + cMargin, gg.a1, fontSize)("ver 0.1");
	ui->Make<xx::Label>()->Init(2, gg.p3 + XY{ -cMargin, cMargin }, gg.a3, fontSize)(gg.lang(Strs::escBack));

	uiEquipBag = ui->Make<UI::EquipBag>();
	uiEquipBag->Init(3);

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
			monsters.Make<Monster>()->Init(this, { x + r_2, y + r_2 }, r);
		}
	}
	monsters.Make<Monster>()->Init(this, { -600, 0 }, 128);
	monsters.Make<Monster>()->Init(this, { 600, 0 }, 128);
}

void Scene_Play::Update() {
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

void Scene_Play::FixedUpdate() {
	for (auto& m : monsters) {
		m->Update();
	}
}

void Scene_Play::Draw() {
	for (auto& m : monsters) {
		m->Draw();
	}

	gg.DrawNode(ui);
}

void Scene_Play::OnResize() {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
