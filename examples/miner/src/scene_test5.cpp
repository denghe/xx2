#include "pch.h"
#include "scene_test5.h"
#include "scene_mainmenu.h"

namespace Test5 {

Monster0& Monster0::Monster0Init(Scene* scene_, XY pos_, float resRadius_, float radius_, float frameDelay_, float speedScale_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	resRadius = resRadius_;
	radius = radius_;
	frameDelay = frameDelay_;
	speedScale = speedScale_;
	return *this;
}

void Monster0::SetAnim(AnimTypes t) {
	switch (t) {
	case AnimTypes::Idle:
		tfs = gg.tf.monster2_idle_.data();
		tfsLen = gg.tf.monster2_idle_.size();
		//aps = gg.ap.monster2_idle_.data();
		cds = {};
		break;
	case AnimTypes::Move:
		tfs = gg.tf.monster2_move_.data();
		tfsLen = gg.tf.monster2_move_.size();
		//aps = gg.ap.monster2_move_.data();
		cds = {};
		break;
	case AnimTypes::Atk:
		tfs = gg.tf.monster2_atk_.data();
		tfsLen = gg.tf.monster2_atk_.size();
		//aps = gg.ap.monster2_atk_.data();
		cds = gg.cd.monster2_atk_.data();
		break;
	}
	tfIndex = 0;
}

bool Monster0::StepAnimOnce() {
	tfIndex += frameDelay * speedScale;
	return tfIndex >= tfsLen;
}

void Monster0::StepAnimLoop() {
	tfIndex += frameDelay * speedScale;
	while (tfIndex >= tfsLen) {
		tfIndex -= tfsLen;
	}
}

bool Monster0::IsHitFrame() const {
	return cds[(int32_t)tfIndex] > 0;
}

void Monster0::Update() {
	StepAnimLoop();
}

void Monster0::Draw() {
	auto& c = scene->cam;
	auto i = (int32_t)tfIndex;
	auto& f = tfs[i];
	//auto ap = aps[i];
	XY ap{ 0.5f, 0.f };
	auto s = radius / resRadius;
	gg.Quad().Draw(f, f, c.ToGLPos(pos), ap, s * c.scale);
	//if (cds && cds[i].to.x > 0) {
	//	auto leftTopPos = pos - XY{ f.uvRect.w * s * ap.x, f.uvRect.h * s * (1.f - ap.y)};
	//	auto p = leftTopPos + cds[i].from * s;
	//	auto siz = cds[i].to - cds[i].from;
	//	auto& o = gg.embed.shape_dot;
	//	gg.Quad().Draw(o, o, c.ToGLPos(p), { 0,1 }, siz * s * c.scale, 0, 1, {255,255,255,222});
	//}
}

/***************************************************************************************/

Monster2& Monster2::Monster2Init(Scene* scene_, XY pos_, float radius_) {
	Monster0Init(scene_, pos_, 50, radius_, 30.f / gg.cFps, 1);
	attackRange = 26.f;
	moveSpeed = 100.f / gg.cFps;
	return *this;
}

void Monster2::Update() {
	XX_BEGIN(_1);
LabSearch:
	SetAnim(AnimTypes::Idle);
LabSearchLoop:
	// simulate spending time thinking
	stepTime = (0.2f + gg.rnd.Next<float>(0.3f)) / speedScale + scene->time;
	while (stepTime > scene->time) {
		StepAnimLoop();
		XX_YIELD(_1);
	}
	// search
	if (!SearchTarget()) goto LabSearchLoop;
LabMove:
	// move to target pos. step cost 0.5s
	SetAnim(AnimTypes::Move);
LabMoveLoop:
	stepTime = 0.5f / speedScale + scene->time;
	while (stepTime > scene->time) {
		flipX = targetPos.x < pos.x;
		StepAnimLoop();
		XX_YIELD(_1);
		if (!target) goto LabSearch;
		auto d = targetPos - pos;
		auto mag2 = d.x * d.x + d.y * d.y;
		auto moveStep = moveSpeed * speedScale;
		if (mag2 < moveStep) {
			flipX = target->pos.x < pos.x;
			goto LabAttack;	// reached
		}
		pos += d / std::sqrtf(mag2) * moveStep;	// move
		y = pos.y;
	}
	goto LabMoveLoop;
LabAttack:
	SetAnim(AnimTypes::Atk);
	hited = false;
	while (!StepAnimOnce()) {
		XX_YIELD(_1);
		if (!target) goto LabSearch;
		if (IsHitFrame()) {
			// todo: sound? effect?
			// todo: hit logic
			hited = true;
		}
	}
	goto LabAttack;
	XX_END(_1);
}

bool Monster2::SearchTarget() {
	target.Reset();
	float minMag2 = std::numeric_limits<float>::max();
	for (auto& o : scene->rocks) {	// todo: optimize by Grid
		auto d = o->pos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		if (dd < minMag2) {
			minMag2 = dd;
			target = o;
		}
	}
	if (!target) return false;
	if (target->pos.x > pos.x) {
		targetPos = target->pos + xx::GetRndPosDoughnut(gg.rnd, attackRange, attackRange * 0.7f
			, 155 / 360.f * M_PI * 2, 200 / 360.f * M_PI * 2);
	}
	else {
		targetPos = target->pos + xx::GetRndPosDoughnut(gg.rnd, attackRange, attackRange * 0.7f
			, -40 / 360.f * M_PI * 2, 5 / 360.f * M_PI * 2);
	}
	return true;
}

void Monster2::Draw() {
	auto& c = scene->cam;
	auto i = (int32_t)tfIndex;
	auto& f = tfs[i];
	//auto ap = aps[i];
	XY ap{ 0.5f, 0.f };
	XY s{ radius / resRadius * c.scale };
	if (flipX) s.x = -s.x;
	gg.Quad().Draw(f, f, c.ToGLPos(pos), ap, s);
}

/***************************************************************************************/

Rock& Rock::Init(Scene * scene_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	radius = radius_;
	return *this;
}

void Rock::Update() {
	// todo: color white effect
	// todo: hit bounce effect
}

void Rock::Draw() {
	auto& c = scene->cam;
	auto& f = gg.tf.rocks_[0][0];
	gg.Quad().Draw(f, f, c.ToGLPos(pos), { 0.5f, 0.1f }, radius / 64.f * c.scale);
}


/***************************************************************************************/

void Scene::Init() {
	cam.Init(gg.scale, 1.f);
	ui.Emplace()->InitRoot(gg.scale * cUIScale);

	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, {-100, -400}, 50, 46, 30.f / gg.cFps, 1).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, { 0, -400 }, 50, 46, 30.f / gg.cFps, 1).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, {100, -400}, 50, 46, 30.f / gg.cFps, 1).SetAnim(AnimTypes::Atk);

	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, {-100, -300}, 50, 46, 30.f / gg.cFps, 2).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, { 0, -300 }, 50, 46, 30.f / gg.cFps, 2).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, {100, -300 }, 50, 46, 30.f / gg.cFps, 2).SetAnim(AnimTypes::Atk);

	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, {-100, -200}, 50, 46, 30.f / gg.cFps, 3).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, { 0, -200 }, 50, 46, 30.f / gg.cFps, 3).SetAnim(AnimTypes::Move);
	monsters.Emplace().Emplace<Monster0>()->Monster0Init(this, {100, -200}, 50, 46, 30.f / gg.cFps, 3).SetAnim(AnimTypes::Atk);

#if 0
	rocks.Emplace().Emplace<Rock>()->Init(this, { 0, 0 }, 23);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 0 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, -50 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 50 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 100 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { -200, 150 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 200, 0 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 200, -50 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 200, 50 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 200, 100 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 200, 150 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 0, -100 }, 23).SetAnim(AnimTypes::Idle);
	monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, { 0, 100 }, 23).SetAnim(AnimTypes::Idle);
#else
	xx::FromTo<XY> posRange{ -gg.designSize / 2 * 0.8, gg.designSize / 2 * 0.8 };
	for (size_t i = 0; i < 100; i++) {
		XY pos{ gg.rnd.Next(posRange.from.x, posRange.to.x), gg.rnd.Next(posRange.from.y, posRange.to.y) };
		rocks.Emplace().Emplace<Rock>()->Init(this, pos, 23);
	}
	for (size_t i = 0; i < 1000; i++) {
		XY pos{ gg.rnd.Next(posRange.from.x, posRange.to.x), gg.rnd.Next(posRange.from.y, posRange.to.y) };
		monsters.Emplace().Emplace<Monster2>()->Monster2Init(this, pos, 23).SetAnim(AnimTypes::Idle);
	}
#endif
}

void Scene::Update() {
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

void Scene::FixedUpdate() {
	if (timer <= time) {
		timer += 2.f;
		std::sort((OrderByYItem**)rocks.buf, (OrderByYItem**)rocks.buf + rocks.len, [](auto& a, auto& b) { return a->y < b->y; });
		std::sort((OrderByYItem**)monsters.buf, (OrderByYItem**)monsters.buf + monsters.len, [](auto& a, auto& b) { return a->y < b->y; });
	}

	for (auto& o : monsters) o->Update();
	for (auto& o : rocks) o->Update();
}

void Scene::Draw() {
	// sort order by y
	assert(obyis.Empty());
	for (auto& o : monsters) obyis.Emplace(o->y, o.pointer);
	for (auto& o : rocks) obyis.Emplace(o->y, o.pointer);
	std::sort(obyis.buf, obyis.buf + obyis.len, [](auto& a, auto& b) { return a.first < b.first; });

	// draw by y
	for (auto& o : obyis) o.second->Draw();
	obyis.Clear();

	gg.GLBlendFunc(gg.blendDefault);
	gg.DrawNode(ui);
}

void Scene::OnResize(bool modeChanged_) {
	ui->Resize(gg.scale * cUIScale);
	cam.SetBaseScale(gg.scale);
}

}
