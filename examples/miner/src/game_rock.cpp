#include "pch.h"
#include "game_rock.h"
#include "game_scene.h"

/********************************************************************************************************/

void BorningRock::Init(Scene* scene_) {
	scene = scene_;
	typeId = gg.rnd.Next<int32_t>(0, gg.all_rocks_().size());
	qualityId = gg.rnd.Next<int32_t>(2);
	f = gg.all_rocks_()[typeId][qualityId * 2 + 1];

	auto fpIdx = gg.rnd.Next<int32_t>(scene->rocksFixedPosPool.len);
	fixedPos = scene->rocksFixedPosPool[fpIdx];
	scene->rocksFixedPosPool.SwapRemoveAt(fpIdx);
	XY posOffset{
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.x, scene->cRockMarginOffsetRange.x),
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.y, scene->cRockMarginOffsetRange.y)
	};
	pos = fixedPos + posOffset;
	y = pos.y;
	centerPos = pos + scene->cRocksPivotOffset;
	flip = gg.rnd.Next<bool>();
	scale = 0;
}

bool BorningRock::Update() {
	static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.25f) };
	if (scale.x < 1.f) {
		scale.x += cScaleStep;
		scale.y = scale.x;
		return false;
	}
	scale = 1.f;
	scene->rocks.Emplace().Emplace<Rock>()->Init(this);
	return true;
}

void BorningRock::Draw() {
	XY s{ scene->cRocksScale * scale * scene->cam.scale };
	if (flip) s.x = -s.x;
	gg.Quad().DrawFrame(f, scene->cam.ToGLPos(pos), s);
}

BorningRock::~BorningRock() {
	if (fixedPos.IsZeroSimple()) return;
	scene->rocksFixedPosPool.Emplace(fixedPos);
}

/********************************************************************************************************/

void BreakingRock::Init(Rock* rock_) {
	scene = rock_->scene;
	pos = rock_->centerPos;
	y = pos.y;
	scale = scene->cRocksScale * 3.f;
	breakFrameIndex = 0.f;
}

bool BreakingRock::Update() {
	static constexpr auto n = gg.all.explosion_1_.size();
	if (breakFrameIndex < n) {
		breakFrameIndex += (gg.cDelta * n / 0.5f);
	}
	return breakFrameIndex >= n;
}

void BreakingRock::Draw() {
	auto& f = gg.all.explosion_1_[breakFrameIndex];
	gg.Quad().DrawFrame(f, scene->cam.ToGLPos(pos), scale * scene->cam.scale);
}

/********************************************************************************************************/

void Pickaxe::Init(Rock* target_) {
	pos = target_->centerPos + XY{ Scene::cRockRadius * target_->scene->cRocksScale, 0 };
	radians = {};
	_1 = {};
}

bool Pickaxe::Update() {
	static constexpr float cStep1Radians{ 30.f / 180.f * M_PI };
	static constexpr float cStep1RadiansStep{ cStep1Radians / (gg.cFps * 0.1f) };
	static constexpr float cStep2Radians{ -50.f / 180.f * M_PI };
	static constexpr float cStep2RadiansStep{ (cStep2Radians - cStep1Radians) / (gg.cFps * 0.2f) };

	XX_BEGIN(_1);
	for (radians = 0; radians < cStep1Radians; radians += cStep1RadiansStep) {
		XX_YIELD_F(_1);
	}
	gg.PlayAudio(gg.ss.pickaxe);
	for (; radians > cStep2Radians; radians += cStep2RadiansStep) {
		XX_YIELD_F(_1);
	}
	return true;
	XX_END(_1);
	return false;
}

void Pickaxe::Draw(Scene* scene_) {
	auto& c = scene_->cam;
	gg.Quad().DrawFrame(gg.all.pickaxe, c.ToGLPos(pos), scene_->cRocksScale * c.scale, radians);
}

/********************************************************************************************************/

bool Rock::Hit(int32_t dmg_) {
	hp -= dmg_;
	if (hp <= cHPMax / 2) {
		f = gg.all_rocks_()[typeId][qualityId * 2];
	}
	if (hp <= 0) {
		Break();
		return true;
	}
	else {
		BeginWhite();
		BeginBounce();
		return false;
	}
}

void Rock::BeginDig() {
	assert(!digging);
	digging = true;
	pickaxe.Init(this);
}

void Rock::BeginBounce() {
	bouncing = true;
	_2 = 0;
	scale = 1.f;
}

void Rock::BeginWhite() {
	whiteEndTime = scene->time + 0.05f;
}

void Rock::Bounce() {
	static constexpr float cScaleStep{ 0.1f / (gg.cFps * 0.133333f) };
	XX_BEGIN(_2);
	for (scale.x = 1.f; scale.x < 1.1f; scale.x += cScaleStep) {
		scale.y = 2.f - scale.x;
		XX_YIELD(_2);
	}
	for (; scale.x > 0.95f; scale.x -= cScaleStep) {
		scale.y = 2.f - scale.x;
		XX_YIELD(_2);
	}
	for (; scale.x < 1.f; scale.x += cScaleStep) {
		scale.y = 2.f - scale.x;
		XX_YIELD(_2);
	}
	scale = 1.f;
	bouncing = false;
	XX_END(_2);
}

void Rock::Break() {
	scene->flyingRocks.Emplace().Init(this);
	scene->breakingRocks.Emplace().Init(this);
	gg.PlayAudio(gg.ss.rockbreak);
	++scene->rocksDisposedCountPerFrame;
}

void Rock::Dispose() {
	assert(indexAtList > -1);
	assert(scene->rocks[indexAtList].pointer == this);
	auto ial = indexAtList;
	scene->rocks.Back()->indexAtList = ial;
	scene->rocks.SwapRemoveAt(ial);
}

void Rock::Init(BorningRock* src_) {
	// move data
	*(BorningRock*)this = *src_;
	src_->fixedPos = {};
	hp = 100;
	scene->rocksGrid.Add(indexAtGrid, this);
	indexAtList = scene->rocks.len - 1;
}

bool Rock::Update() {
	XX_BEGIN(_1);
	while (true) {
		if (bouncing) {
			Bounce();
		}
		XX_YIELD_F(_1);
		if (digging) {
			if (pickaxe.Update()) {
				if (Hit(25)) return true;
				digging = false;
				BeginWhite();
				BeginBounce();
			}
		}
	}
	XX_END(_1);
	return false;
}

void Rock::Draw() {
	XY s{ scene->cRocksScale * scale * scene->cam.scale };
	float cp;
	if (whiteEndTime > scene->time) cp = 100000.f;
	else cp = 1.f;
	if (flip) s.x = -s.x;
	gg.Quad().DrawFrame(f, scene->cam.ToGLPos(pos), s, 0, cp);
	if (digging) {
		pickaxe.Draw(scene);
	}
}

Rock::~Rock() {
	assert(indexAtGrid > -1);
	scene->rocksGrid.Remove(indexAtGrid, this);
}

/********************************************************************************************************/

void FlyingRock::Init(Rock* rock_) {
	scene = rock_->scene;
	pos = rock_->centerPos;
	typeId = rock_->typeId;
	qualityId = rock_->qualityId;
	//auto target = rock_->scene->flyTargets[typeId];
	auto target = scene->minecart->flyTargetPos;
	auto d = target - pos;
	auto mag2 = d.x * d.x + d.y * d.y;
	assert(mag2 > 0);
	auto mag = std::sqrtf(mag2);
	auto norm = d / mag;
	inc = norm * cFlySpeed;
	moveCount = mag / cFlySpeed;
	assert(moveCount > 0);
	moveIndex = 0;
	scale = 0;
	//scaleStep = (50 * 1.5f / 128.f - scene->cRocksScale) / moveCount;
	scaleStep = (scene->minecart->cRocksScale - scene->cRocksScale) / moveCount;
}

bool FlyingRock::Update() {
	XX_BEGIN(_1);
	for (; scale < scene->cRocksScale; scale += cScaleStep * scene->cRocksScale) {
		XX_YIELD_F(_1);
	}
	scale = scene->cRocksScale;
	for (; moveIndex < moveCount; ++moveIndex) {
		pos += inc;
		scale += scaleStep;
		XX_YIELD_F(_1);
	}
	return true;
	XX_END(_1);
	return false;
}

void FlyingRock::Draw(Scene* scene_) {
	auto& c = scene_->cam;
	auto& f = gg.all_rocks_()[typeId][4];
	gg.Quad().DrawFrame(f, c.ToGLPos(pos), scale * c.scale);
}
