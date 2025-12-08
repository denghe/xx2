#include "pch.h"
#include "game_scene.h"
#include "game_porter.h"

Porter& Porter::Init(Scene* scene_, XY pos_) {
	scene = scene_;
	pos = pos_;
	y = pos.y;
	auto& f = gg.all.porter1;
	auto fs = f.Size();
	resRadius = std::min(fs.x, fs.y) / 2.f;
	radius = 32.f;
	moveSpeed = 300.f / gg.cFps;
	catchRange = 100.f;
	rocksMaxLen = 10;
	rocksBasePos = { fs.x / 2.f - fs.x * f.anchor.x, -fs.y };
	CalcCatchingPos();
	return *this;
}

void Porter::Draw() {
	auto& f = gg.all.porter1;
	auto& c = scene->cam;
	XY s{ radius / resRadius * c.scale };
	if (flipX) s.x = -s.x;
	gg.Quad().DrawFrame(f, c.ToGLPos(pos), s);
	auto screenMinY = gg.size_2.y;
	for (auto& o : stackedRocks) {
		auto p = o.CalcDrawPos(this);
		if (p.y > screenMinY)
			break;
		o.Draw(scene, p);
	}
	for (auto& o : catchingRocks) o.Draw(scene);
}

bool Porter::Update() {
	for (auto i = catchingRocks.len - 1; i >= 0; --i) {
		if (catchingRocks[i].Update(this)) {
			catchingRocks.SwapRemoveAt(i);
		}
	}

	XX_BEGIN(_1);
LabSearch:
	//SetAnim(AnimTypes::Idle);
LabSearchLoop:
	// simulate spending time thinking
	stepTime = 0.2f + scene->time;
	while (stepTime > scene->time) {
		//StepAnimLoop();
		XX_YIELD_F(_1);
	}
	// search
	if (!SearchTarget()) goto LabSearchLoop;
LabMove:
	// move to target pos. step cost 0.5s
	//SetAnim(AnimTypes::Move);
LabMoveLoop:
	stepTime = 0.5f / speedScale + scene->time;
	while (stepTime > scene->time) {
		flipX = targetPos.x < pos.x;
		//StepAnimLoop();
		XX_YIELD_F(_1);
		CatchFlyingRocks();
		// todo: if max? move to minecart?
		if (MoveToTarget()) goto LabSearch;
	}
	goto LabMoveLoop;
	XX_END(_1);
	return false;
}

void Porter::CalcCatchingPos() {
	auto& f = gg.all.porter1;
	auto fs = f.Size();
	if (flipX) {
		catchingPos = rocksBasePos.FlipX() + pos;
	}
	else {
		catchingPos = rocksBasePos + pos;
	}
}

XX_INLINE bool Porter::MoveToTarget() {
	// todo: Avoid overlapping together

	if (!target) return true;
	auto d = targetPos - pos;
	auto mag2 = d.x * d.x + d.y * d.y;
	auto moveStep = moveSpeed * speedScale;
	if (mag2 < moveStep) {
		flipX = target->pos.x < pos.x;
		return true;	// reached
	}
	pos += d / std::sqrtf(mag2) * moveStep;	// move
	y = pos.y;
	CalcCatchingPos();
	return false;
}

void Porter::CatchFlyingRocks() {
	// todo: max limit

	// range catch
	auto& g = scene->collectingRocksGrid;
	auto cri = g.PosToCRIndex(pos);
	auto rockRadius = scene->cRockRadius * scene->cRocksScale;
	auto r = catchRange + rockRadius;
	auto rr = r * r;
	g.ForeachByRange(cri.y, cri.x, catchRange + rockRadius * 3, gg.sgrdd, [&](xx::Grid2dCircle<CollectingRock*>::Node& node, float distance)->bool {
		auto o = node.value;
		auto d = o->pos - pos;
		auto mag2 = d.x * d.x + d.y * d.y;
		if (mag2 < rr) {
			o->CatchBy(this);
		}
		return false;
	});
}

bool Porter::SearchTarget() {
	target.Reset();
#if 1
	float minMag2 = std::numeric_limits<float>::max();
	if (scene->collectingRocks.len < 10) {
		for (auto& o : scene->collectingRocks) {
			auto d = o->pos - pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			if (mag2 < minMag2) {
				minMag2 = mag2;
				target = o;
			}
		}
	}
	else {
		auto& g = scene->collectingRocksGrid;
		auto cri = g.PosToCRIndex(pos);
		auto searchRange = gg.designSize.x;
		auto rockRadius = scene->cRockRadius * scene->cRocksScale;
		auto r = searchRange + rockRadius;
		auto rr = r * r;
		int32_t limit{ 5 };
		g.ForeachByRange(cri.y, cri.x, searchRange + rockRadius * 3, gg.sgrdd, [&](xx::Grid2dCircle<CollectingRock*>::Node& node, float distance)->bool {
			auto o = node.value;
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
#else
	if (scene->collectingRocks.len > 0) {
		target = gg.rnd.NextElement(scene->collectingRocks);
	}
#endif
	if (!target) return false;
	targetPos = target->pos;
	return true;
}
