#include "pch.h"
#include "scene_1.h"

void Scene_1::Init() {
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
}

void Scene_1::OnResize() {
	cam.SetBaseScale(gg.scale);
}
