#include "pch.h"
#include "game_rock.h"
#include "game_porter.h"
#include "game_scene.h"

/********************************************************************************************************/
// Pickaxe

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
// BorningRock

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
// Rock

bool Rock::Hit(int32_t dmg_) {
	hp -= dmg_;
	if (hp <= cHPMax / 2) {
		f = gg.all_rocks_()[typeId][qualityId * 2];
	}
	if (hp <= 0) {
		scene->flyingRocks.Emplace().Emplace()->Init(this);
		scene->breakingRocks.Emplace().Init(this);
		gg.PlayAudio(gg.ss.rockbreak);
		++scene->rocksDisposedCountPerFrame;
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

void Rock::Dispose() {
	assert(indexAtList > -1);
	assert(scene->rocks[indexAtList].pointer == this);
	auto ial = indexAtList;
	scene->rocks.Back()->indexAtList = ial;
	scene->rocks.SwapRemoveAt(ial);
}

void Rock::Init(BorningRock* src_) {
	*(BorningRock*)this = *src_;	// move logic
	src_->fixedPos = {};	// avoid src_ release
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
// BreakingRock

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
// FlyingRock

void FlyingRock::Init(Rock* rock_) {
	scene = rock_->scene;
	pos = rock_->pos;
	y = pos.y;
	typeId = rock_->typeId;
	qualityId = rock_->qualityId;
	scene->flyingRocksGrid.Add(indexAtGrid, this);
	indexAtList = scene->flyingRocks.len - 1;
}

bool FlyingRock::Update() {
	// fade in
	static constexpr auto cAlphaStep{ 1.f / (gg.cFps * 0.5f) };
	if (alpha < 1.f) {
		alpha += cAlphaStep;
	}
	else {
		alpha = 1.f;
	}
	// todo: flash effect
	return false;
}

void FlyingRock::Draw() {
	auto& c = scene->cam;
	auto& f = gg.all_rocks_()[typeId][4];
	gg.Quad().DrawFrame(f, c.ToGLPos(pos), scene->cRocksScale * c.scale * 0.8f
		, 0, 1.f, {255,255,255,(uint8_t)(255.f * alpha)});
}

void FlyingRock::Dispose() {
	assert(indexAtList > -1);
	assert(scene->flyingRocks[indexAtList].pointer == this);
	auto ial = indexAtList;
	scene->flyingRocks.Back()->indexAtList = ial;
	scene->flyingRocks.SwapRemoveAt(ial);
}

void FlyingRock::CatchBy(Porter* owner_) {
	owner_->catchingRocks.Emplace().Init(this);
	assert(indexAtGrid > -1);
	scene->flyingRocksGrid.Remove(indexAtGrid, this);
	Dispose();
}

FlyingRock::~FlyingRock() {
	if(indexAtGrid > -1) {
		scene->flyingRocksGrid.Remove(indexAtGrid, this);
	}
}

/********************************************************************************************************/
// CatchingRock

void CatchingRock::Init(FlyingRock* flyingRock_) {
	pos = flyingRock_->pos;
	typeId = flyingRock_->typeId;
	qualityId = flyingRock_->qualityId;
	flySpeed = 500.f / gg.cFps;
}

bool CatchingRock::Update(Porter* porter_) {
	// fly to porter_
	auto d = porter_->catchingPos - pos;
	auto mag2 = d.x * d.x + d.y * d.y;
	if (mag2 < flySpeed * flySpeed) {	// reached
		porter_->stackedRocks.Emplace().Init(porter_, this);
		return true;
	}
	pos += d / std::sqrtf(mag2) * flySpeed;
	flySpeed += 0.1f;
	return false;
}

void CatchingRock::Draw(Scene* scene_) {
	auto& c = scene_->cam;
	auto& f = gg.all_rocks_()[typeId][4];
	gg.Quad().DrawFrame(f, c.ToGLPos(pos), scene_->cRocksScale * c.scale * 0.8f);
}

/********************************************************************************************************/
// StackedRock

XY StackedRock::CalcDrawPos(Porter* porter_) const {
	if (porter_->flipX) {
		return porter_->scene->cam.ToGLPos(porter_->pos + pos.FlipX());
	}
	else {
		return porter_->scene->cam.ToGLPos(porter_->pos + pos);
	}
}

void StackedRock::Init(Porter* porter_, CatchingRock* catchingRock_) {
	pos = porter_->rocksBasePos;
	// todo: + offset
	porter_->rocksBasePos += XY{ 0, -5.f };
	typeId = catchingRock_->typeId;
	qualityId = catchingRock_->qualityId;
}

void StackedRock::Draw(Scene* scene_, XY p_) {
	auto& c = scene_->cam;
	auto& f = gg.all_rocks_()[typeId][4];
	gg.Quad().DrawFrame(f, p_, scene_->cRocksScale * c.scale * 0.8f);
}
