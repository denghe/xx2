#include "pch.h"
#include "scene_test4.h"
#include "scene_mainmenu.h"


Monster1& Monster1::Init(Scene_Test4* scene_, XY pos_, float speedScale_) {
	scene = scene_;
	pos = pos_;
	speedScale = speedScale_;
	return *this;
}

void Monster1::SetAnim(AnimTypes t) {
	switch (t) {
	case AnimTypes::Idle:
		tfs = gg.tf.monster1_idle_.data();
		tfsLen = gg.tf.monster1_idle_.size();
		break;
	case AnimTypes::Move:
		tfs = gg.tf.monster1_move_.data();
		tfsLen = gg.tf.monster1_move_.size();
		break;
	case AnimTypes::Atk:
		tfs = gg.tf.monster1_atk_.data();
		tfsLen = gg.tf.monster1_atk_.size();
		break;
	}
	tfIndex = 0;
}

bool Monster1::Update() {
	tfIndex += (15.f / gg.cFps) * speedScale;
	// todo: hit check event
	while (tfIndex >= tfsLen) {
		tfIndex -= tfsLen;
	}
	//if (tfIndex >= tfsLen) {
	//	SetAnim(AnimTypes::Idle);
	//}
	return false;
}

void Monster1::Draw() {
	auto& f = tfs[(int32_t)tfIndex];
	auto& c = scene->cam;
	gg.Quad().Draw(f, f, c.ToGLPos(pos), { 0.5f, 0 }, 1.f * c.scale);
}

/***************************************************************************************/

void Scene_Test4::Init() {
	cam.Init(gg.scale, 2.f);
	ui.Emplace()->InitRoot(gg.scale * cUIScale);

	monsters.Emplace().Emplace<Monster1>()->Init(this, {-100, -100}).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster1>()->Init(this, { 0, -100 }).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster1>()->Init(this, {100, -100}).SetAnim(AnimTypes::Atk);

	monsters.Emplace().Emplace<Monster1>()->Init(this, {-100, 0}, 2.f).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster1>()->Init(this, { 0, 0 }, 2.f).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster1>()->Init(this, {100, 0}, 2.f).SetAnim(AnimTypes::Atk);

	monsters.Emplace().Emplace<Monster1>()->Init(this, {-100, 100}, 3.f).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster1>()->Init(this, { 0, 100 }, 3.f).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster1>()->Init(this, {100, 100}, 3.f).SetAnim(AnimTypes::Atk);
}

void Scene_Test4::Update() {
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

void Scene_Test4::FixedUpdate() {
	// drive
	for (auto i = monsters.len - 1; i >= 0; --i) {
		if (monsters[i]->Update()) {
			monsters.SwapRemoveAt(i);
		}
	}
}

void Scene_Test4::Draw() {
	for (auto& o : monsters) o->Draw();

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test4::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}
