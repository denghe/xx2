#include "pch.h"
#include "game_rock.h"
#include "game_flyingrock.h"
#include "game_scene.h"

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
	gg.Quad().Draw(gg.tf.pickaxe, gg.tf.pickaxe, scene_->cam.ToGLPos(pos)
		, { 0.5f, 0.25f }, scene_->cRocksScale * scene_->cam.scale, radians);
}

/********************************************************************************************************/

void Rock::BeginDig() {
	assert(!mining);
	mining = true;
	pickaxe.Init(this);
}

void Rock::Init(Scene* scene_) {
	gg.sound.SetGlobalVolume(0.1);
	scene = scene_;
	hp = cHPMax;
	typeId = gg.rnd.Next<int32_t>(0, gg.tf.rocks_.size());
	qualityId = gg.rnd.Next<int32_t>(2);
	tf = gg.tf.rocks_[typeId][qualityId * 2 + 1];

	auto fpIdx = gg.rnd.Next<int32_t>(scene->rocksFixedPosPool.len);
	fixedPos = scene->rocksFixedPosPool[fpIdx];
	scene->rocksFixedPosPool.SwapRemoveAt(fpIdx);
	XY posOffset{
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.x, scene->cRockMarginOffsetRange.x),
		gg.rnd.Next<float>(-scene->cRockMarginOffsetRange.y, scene->cRockMarginOffsetRange.y)
	};
	pos = fixedPos + posOffset;
	centerPos = pos + scene->cRocksPivotOffset;
	flip = gg.rnd.Next<bool>();
	scene->rocksGrid.Add(indexAtGrid, this);
}

void Rock::BeginBounce() {
	bouncing = true;
	_2 = 0;
	scale = 1.f;
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

void Rock::Update() {
	static constexpr auto esiz = gg.tf.explosion_1_.size();
	static constexpr float cScaleStep{ 1.f / (gg.cFps * 0.25f) };

	XX_BEGIN(_1);
	for (scale.x = 0; scale.x < 1.f; scale.x += cScaleStep) {
		scale.y = scale.x;
		XX_YIELD(_1);
	}
	scale = 1.f;
	ready = true;
	while (true) {
		if (bouncing) {
			Bounce();
		}
		XX_YIELD(_1);
		if (mining) {
			if (pickaxe.Update()) {
				hp -= 25;
				if (hp <= cHPMax / 2) {
					tf = gg.tf.rocks_[typeId][qualityId * 2];
				}
				if (hp <= 0) goto LabBreak;
				else {
					mining = false;
					BeginBounce();
				}
			}
		}
	}
LabBreak:
	ready = false;
	breaking = true;
	scene->flyingRocks.Emplace().Init(this);
	gg.PlayAudio(gg.ss.rockbreak);
	for (breakFrameIndex = 0.f; breakFrameIndex < esiz; breakFrameIndex += (gg.cDelta * esiz / 0.5f)) {
		XX_YIELD(_1);
	}
	++scene->rocksDisposedCountPerFrame;
	Dispose();
	return;
	XX_END(_1);
}

void Rock::Draw() {
	XY s{ scene->cRocksScale * scale * scene->cam.scale };
	if (breaking) {
		auto& f = gg.tf.explosion_1_[breakFrameIndex];
		gg.Quad().Draw(f, f, scene->cam.ToGLPos(centerPos), 0.5f, s * 3.f);
	}
	else {
		if (flip) s.x = -s.x;
		gg.Quad().Draw(tf, tf, scene->cam.ToGLPos(pos), { 0.5f, 0 }, s);
		if (mining) {
			pickaxe.Draw(scene);
		}
	}
}

void Rock::Dispose() {
	assert(indexAtList > -1);
	assert(scene->rocks[indexAtList].pointer == this);
	auto ial = indexAtList;
	scene->rocks.Back()->indexAtList = ial;
	scene->rocks.SwapRemoveAt(ial);
}

Rock::~Rock() {
	assert(indexAtGrid > -1);
	scene->rocksGrid.Remove(indexAtGrid, this);
	scene->rocksFixedPosPool.Emplace(fixedPos);
}

