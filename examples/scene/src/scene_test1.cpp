#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Bug::StaticInit() {
	scales[0] = cScale0;
	steps[0] = cStep0;
	for (int i = 1; i < cLen; ++i) {
		steps[i] = steps[i - 1] * cStepMul;
		scales[i] = scales[i - 1] * cStepMul;
	}
}

void Bug::Init(Scene_Test1* scene_, XY pos_, XY tar_, xx::RGBA8 color_) {
	scene = scene_;
	color = color_;
	poss[0] = pos_;
	auto d = poss[0] - tar_;
	auto r = radianss[0] = std::atan2f(d.y, d.x);
	XY inc{ std::cosf(r), std::sinf(r) };
	for (int i = 1; i < cLen; ++i) {
		poss[i] = poss[i - 1] + inc * steps[i];
		radianss[i] = r;
	}
}

void Bug::Update(XY tar_) {
	// pos -> mouse
	{
		auto d = tar_ - poss[0];
		auto r = radianss[0] = std::atan2f(d.y, d.x);
		XY inc{ std::cosf(r), std::sinf(r) };
		poss[0] += inc * cSpeed;
	}
	for (int i = 1; i < cLen; ++i) {
		auto& tar = poss[i - 1];
		auto d = tar - poss[i];
		auto r = radianss[i] = std::atan2f(d.y, d.x);
		XY inc{ std::cosf(r), std::sinf(r) };
		poss[i] = tar - inc * steps[i];
	}
}

void Bug::Draw() {
	auto& c = scene->cam;
#if 0
	gg.Quad().DrawFrame(gg.fs.bug_head, poss[0] * c.scale, scales[0] * c.scale, radianss[0], 1.f, color);
	for (int i = 1; i < cLen; ++i) {
		gg.Quad().DrawFrame(gg.fs.bug_body, poss[i] * c.scale, scales[i] * c.scale, radianss[i], 1.f, color);
	}
#else
	auto& f0 = gg.fs.bug_head;
	auto buf = gg.Quad().Alloc(f0, cLen);
	buf[0].Fill(f0, poss[0] * c.scale, f0, scales[0] * c.scale, radianss[0], 1.f, color);
	auto& f1 = gg.fs.bug_body;
	for (int i = 1; i < cLen; ++i) {
		buf[i].Fill(f1, poss[i] * c.scale, f1, scales[i] * c.scale, radianss[i], 1.f, color);
	}
#endif
}

/***************************************************************************/

void Scene_Test1::Init() {
	ui.Emplace()->InitRoot(gg.scale);
	cam.Init(gg.scale, 1.f, 0);

	Bug::StaticInit();
	auto ds2 = gg.designSize * 0.5f;
	auto tar = gg.mousePos / cam.baseScale;
	for (int i = 0; i < 10000; ++i) {
		XY pos{};
		pos.x = gg.rnd.Next(-ds2.x, ds2.x);
		pos.y = gg.rnd.Next(-ds2.y, ds2.y);
		bugs.Emplace().Emplace()->Init(this, pos, tar, xx::GetRandomColor(gg.rnd, {222,0,0,255}));
	}
}

void Scene_Test1::Update() {
	// handle inputs
	if (gg.keyboard[GLFW_KEY_ESCAPE](0.2f)) {
		gg.MakeScene<Scene_MainMenu>()->Init();
		return;
	}

	// fixed update
	auto d = float(std::min((float)gg.delta, gg.cMaxDelta) * timeScale);
	timePool += d;
	while (timePool >= gg.cDelta) {
		time += gg.cDelta;
		timePool -= gg.cDelta;
		FixedUpdate();
	}
}

void Scene_Test1::FixedUpdate() {
	// ...
	auto tar = gg.mousePos / cam.baseScale;
	for (auto& o : bugs) o->Update(tar);
}

void Scene_Test1::Draw() {
	for (auto& o : bugs) o->Draw();
	// ...
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
