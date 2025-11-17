#include "pch.h"
#include "scene_test6.h"
#include "scene_mainmenu.h"

namespace Test6 {

Monster& Monster::Init(Scene* scene_, int32_t monsterTypeId_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	monsterTypeId = monsterTypeId_;
	radius = radius_;
	speedScale = 1.f;
	auto& mc = gg.mcs[monsterTypeId];
	frameDelay = mc.animFPS / gg.cFps;
	resRadius = mc.resRadius;
	moveSpeed = mc.moveSpeed;
	attackRange = mc.attackRange;
	SetAnim(AnimTypes::Idle);
	return *this;
}

void Monster::SetAnim(AnimTypes t) {
	auto& mc = gg.mcs[monsterTypeId];
	tfs = mc.tfss[(int32_t)t];
	tfsLen = mc.tfsLens[(int32_t)t];
	ap = mc.aps[(int32_t)t];
	if (t == AnimTypes::Atk) {
		cds = mc.cd;
	}
	else {
		cds = {};
	}
	tfIndex = 0;
	
}

bool Monster::StepAnimOnce() {
	tfIndex += frameDelay * speedScale;
	return tfIndex >= tfsLen;
}

void Monster::StepAnimLoop() {
	tfIndex += frameDelay * speedScale;
	while (tfIndex >= tfsLen) {
		tfIndex -= tfsLen;
	}
}

char Monster::GetHitData() const {
	return cds[(int32_t)tfIndex];
}

void Monster::Draw() {
	auto& c = scene->cam;
	auto i = (int32_t)tfIndex;
	auto& f = tfs[i];
	XY s{ radius / resRadius * c.scale };
	if (flipX) s.x = -s.x;
	gg.Quad().Draw(f, f, c.ToGLPos(pos), ap, s);
}

void Monster::Update() {
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
	if (gg.GetActiveVoiceCount() < 32) {
		gg.PlayAudio(gg.mcs[monsterTypeId].ss.Lock(), 0.3f);
	}
	hited = 0;
	while (!StepAnimOnce()) {
		XX_YIELD(_1);
		if (!target) goto LabSearch;
		if (auto r = GetHitData(); r > hited) {
			// todo: sound? effect?
			// todo: hit logic
			hited = r;
		}
	}
	goto LabAttack;
	XX_END(_1);
}

bool Monster::SearchTarget() {
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

	xx::FromTo<XY> posRange{ -gg.designSize / 2 * 0.8, gg.designSize / 2 * 0.8 };
	for (size_t i = 0; i < 1000; i++) {
		XY pos{ gg.rnd.Next(posRange.from.x, posRange.to.x), gg.rnd.Next(posRange.from.y, posRange.to.y) };
		rocks.Emplace().Emplace<Rock>()->Init(this, pos, 23);
	}
	for (size_t i = 0; i < 1000; i++) {
		XY pos{ gg.rnd.Next(posRange.from.x, posRange.to.x), gg.rnd.Next(posRange.from.y, posRange.to.y) };
		auto monsterTypeId = gg.rnd.Next(gg.mcs.size());
		monsters.Emplace().Emplace<Monster>()->Init(this, monsterTypeId, pos, 23);
	}

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
