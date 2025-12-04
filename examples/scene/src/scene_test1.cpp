#include "pch.h"
#include "scene_test1.h"
#include "scene_mainmenu.h"

void Bug::StaticInit() {
	scales[0] = 1.f;
	steps[0] = cStep0;
	for (int i = 1; i < cLen; ++i) {
		steps[i] = steps[i - 1] * cStepMul;
		scales[i] = scales[i - 1] * cStepMul;
	}
}

void Bug::Init(XY pos_, float radians_) {
	poss[0] = pos_;
	radianss[0] = radians_;
	XY inc{ std::cosf(radians_ + M_PI), std::sinf(radians_ + M_PI) };
	for (int i = 1; i < cLen; ++i) {
		poss[i] = poss[i - 1] + inc * steps[i];
		radianss[i] = radians_;
	}
}

void Bug::Update() {
	static constexpr float cSpeed{ 100.f / gg.cFps };
	// pos -> mouse
	{
		auto tar = gg.mousePos;
		auto d = tar - poss[0];
		auto radians = radianss[0] = std::atan2f(d.y, d.x);
		XY inc{ std::cosf(radians), std::sinf(radians) };
		poss[0] += inc * cSpeed;
	}
	for (int i = 1; i < cLen; ++i) {
		auto tar = poss[i - 1];
		auto d = tar - poss[i];
		auto radians = radianss[i] = std::atan2f(d.y, d.x);
		XY inc{ std::cosf(radians), std::sinf(radians) };
		poss[i] = tar - inc * steps[i];
	}
}

void Bug::Draw() {
#if 0
	gg.Quad().Draw(gg.fs.bug_head, poss[0], scales[0], -radianss[0]);
	for (int i = 1; i < cLen; ++i) {
		gg.Quad().Draw(gg.fs.bug_body, poss[i], scales[i], -radianss[i]);
	}
#else
	auto& f0 = gg.fs.bug_head;
	auto buf = gg.Quad().Alloc(f0, cLen);
	buf[0].Fill(f0, poss[0], scales[0], -radianss[0]);
	auto& f1 = gg.fs.bug_body;
	for (int i = 1; i < cLen; ++i) {
		buf[i].Fill(f1, poss[i], scales[i], -radianss[i]);
	}
#endif
}

/***************************************************************************/

void Scene_Test1::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale);

	Bug::StaticInit();
	for (int i = 0; i < 10000; ++i) {
		bugs.Emplace().Emplace()->Init(0, 0);
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
	for (auto& o : bugs) o->Update();
}

void Scene_Test1::Draw() {
	for (auto& o : bugs) o->Draw();
	// ...
	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test1::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale);
	cam.SetBaseScale(gg.scale);
}
