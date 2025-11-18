#include "pch.h"
#include "game_scene.h"
#include "game_monster.h"

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
	if (t == AnimTypes::Atk) cds = mc.cd;
	else cds = {};
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

bool Monster::Update() {
	XX_BEGIN(_1);
LabSearch:
	SetAnim(AnimTypes::Idle);
LabSearchLoop:
	// simulate spending time thinking
	stepTime = (0.2f + gg.rnd.Next<float>(0.3f)) / speedScale + scene->time;
	while (stepTime > scene->time) {
		StepAnimLoop();
		XX_YIELD_F(_1);
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
		XX_YIELD_F(_1);
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
	if (gg.GetActiveVoiceCount() < 8) {
		gg.PlayAudio(gg.mcs[monsterTypeId].ss.Lock(), 0.3f);
	}
	hited = 0;
	while (!StepAnimOnce()) {
		XX_YIELD_F(_1);
		if (target) {
			if (auto r = GetHitData(); r > hited) {
				if (target->Hit(25)) {
					target->Dispose();
				}
				hited = r;
			}
		}
	}
	if (target) goto LabAttack;
	else goto LabSearch;
	XX_END(_1);
	return false;
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
