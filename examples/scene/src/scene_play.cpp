#include "pch.h"
#include "scene_play.h"
#include "scene_mainmenu.h"

// todo: equip + bag + props list + log panel
// game logic: player auto attack monster. monster dead: drop items. monster auto reborn. 

void Scene_Play::Init() {

	// init game logic
	cam.Init(gg.scale, 1.f);

#if 0
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
#endif

	monsters.Make<Monster>()->Init(this, { -80, 0 }, 128);
	player.Emplace<Player>()->Init(this, { 80, 0 }, 128);


	// init ui
	static constexpr float cLineHeight{ 100 };
	static constexpr float cItemHeight{ 80 };
	static constexpr float cMargin{ cLineHeight - cItemHeight };
	auto fontSize = cItemHeight - gg.embed.cfg_s9bN->paddings.TopBottom();
	ui.Emplace()->InitRoot(gg.scale);
	ui->Make<xx::Label>()->Init(2, gg.p1 + cMargin, gg.a1, fontSize)("ver 0.1");
	ui->Make<xx::Label>()->Init(2, gg.p3 + XY{ -cMargin, cMargin }, gg.a3, fontSize)(gg.lang(Strs::escBack));

	uiEquipBag = ui->Make<UI::EquipBag>();
	uiEquipBag->Init(3, player.ToWeak());

	uiPropsLog = ui->Make<UI::PropsLog>();
	uiPropsLog->Init(3, player.ToWeak());

}

void Scene_Play::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	// fixed update
	auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
	time += d;
	timePool += d;
	while (timePool >= gg.cDelta) {
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene_Play::FixedUpdate() {
	// update game logic
	for (auto& m : monsters) {
		m->Update();
	}
	player->Update();
}

void Scene_Play::Draw() {
	// todo: order by Y
	for (auto& m : monsters) {
		m->Draw();
	}
	player->Draw();
	gg.DrawNode(ui);
}

void Scene_Play::OnResize(bool modeChanged_) {
	if (modeChanged_) {
		if (auto o = ui->FindFirstType<UI::Settings>(); o) {
			o->Init(o->z);
		}
	}
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
