#include "pch.h"
#include "game_scene.h"
#include "game_miner.h"

Miner& Miner::Init(Scene* scene_, int32_t minerTypeId_, XY pos_, float radius_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	minerTypeId = minerTypeId_;
	radius = radius_;
	speedScale = 1.f;
	auto& mc = gg.mcs[minerTypeId];
	frameDelay = mc.animFPS / gg.cFps;
	resRadius = mc.resRadius;
	moveSpeed = mc.moveSpeed;
	attackRange = mc.attackRange;
	SetAnim(AnimTypes::Idle);
	return *this;
}

void Miner::SetAnim(AnimTypes t) {
	auto& mc = gg.mcs[minerTypeId];
	fs = mc.fss[(int32_t)t];
	fsLen = mc.fsLens[(int32_t)t];
	if (t == AnimTypes::Atk) cds = mc.cd;
	else cds = {};
	fsCursor = 0;
}

bool Miner::StepAnimOnce() {
	fsCursor += frameDelay * speedScale;
	return fsCursor >= fsLen;
}

void Miner::StepAnimLoop() {
	fsCursor += frameDelay * speedScale;
	while (fsCursor >= fsLen) {
		fsCursor -= fsLen;
	}
}

char Miner::GetHitData() const {
	return cds[(int32_t)fsCursor];
}

void Miner::Draw() {
	auto& c = scene->cam;
	auto i = (int32_t)fsCursor;
	auto& f = fs[i];
	XY s{ radius / resRadius * c.scale };
	if (flipX) s.x = -s.x;
	gg.Quad().DrawFrame(f, c.ToGLPos(pos), s);
	// todo: shadow ?
}

bool Miner::Update() {
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
		gg.PlayAudio(gg.mcs[minerTypeId].ss.Lock(), 0.3f);
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

bool Miner::SearchTarget() {
	target.Reset();
	float minMag2 = std::numeric_limits<float>::max();
	if (scene->rocks.len < 30) {
		for (auto& o : scene->rocks) {
			auto d = o->pos - pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			if (mag2 < minMag2) {
				minMag2 = mag2;
				target = o;
			}
		}
	}
	else {
		auto& g = scene->rocksGrid;
		auto cri = g.PosToCRIndex(pos);
		auto searchRange = scene->cRockRadius * 2;
		auto rockRadius = scene->cRockRadius * scene->cRocksScale;
		auto r = searchRange + rockRadius;
		auto rr = r * r;
		int32_t limit{ 5 };
		g.ForeachByRange(cri.y, cri.x, searchRange + rockRadius * 3, gg.sgrdd, [&](xx::Grid2dCircle<Rock*>::Node& node, float distance)->bool {
			auto o = node.value;
			if (o->digging) return false;
			auto d = o->pos - pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			if (mag2 < rr) {
				if (mag2 < minMag2) {
					minMag2 = mag2;
					target = xx::WeakFromThis(o);
				}
				if (--limit <= 0) return true;
			}
			return false;
		});
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
