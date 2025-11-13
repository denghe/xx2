#include "pch.h"
#include "scene_test4.h"
#include "scene_mainmenu.h"


Monster1& Monster1::Monster1Init(Scene_Test4* scene_, XY pos_, float radius_, float speedScale_) {
	scene = scene_;
	pos = pos_;
	radius = radius_;
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

bool Monster1::StepAnimOnce() {
	tfIndex += (12.f / gg.cFps) * speedScale;
	return tfIndex >= tfsLen;
}

void Monster1::StepAnimLoop() {
	tfIndex += (12.f / gg.cFps) * speedScale;
	while (tfIndex >= tfsLen) {
		tfIndex -= tfsLen;
	}
}

bool Monster1::IsHitFrame() const {
	return cds[(int32_t)tfIndex].to.x > 0;
}

void Monster1::Update() {
	StepAnimLoop();
}

void Monster1::Draw() {
	auto& c = scene->cam;
	auto i = (int32_t)tfIndex;
	auto& f = tfs[i];
	auto ap = aps[i];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), ap, radius / 23.f * c.scale);
	if (cds && cds[i].to.x > 0) {
		auto leftTopPos = pos - XY{ f.uvRect.w * ap.x, f.uvRect.h * (1.f - ap.y)};
		auto p = leftTopPos + cds[i].from;
		auto siz = cds[i].to - cds[i].from;
		auto& o = gg.embed.shape_dot;
		gg.Quad().Draw(o, o, c.ToGLPos(p), { 0,1 }, siz * radius / 23.f * c.scale, 0, 1, {255,255,255,127});
	}
}

/***************************************************************************************/

Monster2& Monster2::Monster2Init(Scene_Test4* scene_, XY pos_, float radius_) {
	Monster1Init(scene_, pos_, radius_, 1);
	attackRange = 16.f;
	moveSpeed = 100.f / gg.cFps;
	return *this;
}

void Monster2::Update() {
	XX_BEGIN(_1);
LabSearch:
	SetAnim(AnimTypes::Idle);
LabSearchWithoutSetAnim:
	// simulate spending time thinking
	stepTime = (0.2f + gg.rnd.Next<float>(0.3f)) / speedScale + scene->time;
	while (stepTime > scene->time) {
		StepAnimLoop();
		XX_YIELD(_1);
	}
	// search
	if (!SearchTarget()) goto LabSearchWithoutSetAnim;
LabMove:
	// move to target. cost 0.5s	// todo: find hit point ( target left or right )
	SetAnim(AnimTypes::Move);
LabMoveWithoutSetAnim:
	stepTime = 0.5f / speedScale + scene->time;
	while (stepTime > scene->time) {
		StepAnimLoop();
		XX_YIELD(_1);
		if (!target) goto LabSearch;
		auto d = target->pos - pos;
		auto r = radius + attackRange;
		auto rr = (target->radius + r) * (target->radius + r);
		auto mag2 = d.x * d.x + d.y * d.y;
		if (rr > mag2) {
			goto LabAttack;	// reached
		}
		if (mag2 > 0.f) {
			auto norm = d / std::sqrtf(mag2);
			pos += norm * moveSpeed * speedScale;	// move
		}
	}
	goto LabMoveWithoutSetAnim;
LabAttack:
	SetAnim(AnimTypes::Atk);
	while (!StepAnimOnce()) {
		XX_YIELD(_1);
		if (!target) goto LabSearch;
		if (IsHitFrame()) {
			// todo: hit logic
		}
	}
	goto LabAttack;
	XX_END(_1);
}

bool Monster2::SearchTarget() {
	target.Reset();
	if (scene->rocks.len) {
		target = scene->rocks.Back();	// todo: search by range
		return true;
	}
	return false;
}

void Monster2::Draw() {
	auto& c = scene->cam;
	auto i = (int32_t)tfIndex;
	auto& f = tfs[i];
	auto ap = aps[i];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), ap, radius / 23.f * c.scale);
}

/***************************************************************************************/

Rock& Rock::Init(Scene_Test4 * scene_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	radius = radius_;
	return *this;
}

void Rock::Update() {
}

void Rock::Draw() {
	auto& c = scene->cam;
	auto& f = gg.tf.rocks_[0][0];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), { 0.5f, 0.f }, radius / 64.f * c.scale);
}


/***************************************************************************************/

void Scene_Test4::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale * cUIScale);

	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, {-100, -400}, 23.f, 1).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, { 0, -400 }, 23.f, 1).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, {100, -400}, 23.f, 1).SetAnim(AnimTypes::Atk);

	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, {-100, -300}, 23.f, 2).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, { 0, -300 }, 23.f, 2).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, {100, -300 }, 23.f, 2).SetAnim(AnimTypes::Atk);

	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, {-100, -200}, 23.f, 3).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, { 0, -200 }, 23.f, 3).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster1>()->Monster1Init(this, {100, -200}, 23.f, 3).SetAnim(AnimTypes::Atk);

	rocks.Emplace().Emplace<Rock>()->Init(this, { 0, 0 }, 23);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 0 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, -50 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 50 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 100 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 150 }, 23).SetAnim(AnimTypes::Idle);
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
	for (auto& o : monsters) o->Update();
	for (auto& o : rocks) o->Update();
}

void Scene_Test4::Draw() {
	// todo: order by y
	for (auto& o : monsters) o->Draw();
	for (auto& o : rocks) o->Draw();

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene_Test4::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}
