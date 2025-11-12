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
		aps = gg.ap.monster1_idle_.data();
		cds = {};
		break;
	case AnimTypes::Move:
		tfs = gg.tf.monster1_move_.data();
		tfsLen = gg.tf.monster1_move_.size();
		aps = gg.ap.monster1_move_.data();
		cds = {};
		break;
	case AnimTypes::Atk:
		tfs = gg.tf.monster1_atk_.data();
		tfsLen = gg.tf.monster1_atk_.size();
		aps = gg.ap.monster1_atk_.data();
		cds = gg.cd.monster1_atk_.data();
		break;
	}
	tfIndex = 0;
}

bool Monster1::Update() {
	tfIndex += (12.f / gg.cFps) * speedScale;
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
	auto& c = scene->cam;
	auto i = (int32_t)tfIndex;
	auto& f = tfs[i];
	auto ap = aps[i];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), ap, c.scale);
	if (cds && cds[i].to.x > 0) {
		auto leftTopPos = pos - XY{ f.uvRect.w * ap.x, f.uvRect.h * (1.f - ap.y)};
		auto p = leftTopPos + cds[i].from;
		auto siz = cds[i].to - cds[i].from;
		auto& o = gg.embed.shape_dot;
		gg.Quad().Draw(o, o, c.ToGLPos(p), { 0,1 }, siz * c.scale, 0, 1, {255,255,255,127});
	}
}

/***************************************************************************************/

void Scene_Test4::Init() {
	cam.Init(gg.scale, 3.f);
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
