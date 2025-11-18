#include "pch.h"
#include "game_scene.h"
#include "game_rock.h"
#include "game_flyingrock.h"

void FlyingRock::Init(Rock* rock_) {
	scene = rock_->scene;
	pos = rock_->centerPos;
	typeId = rock_->typeId;
	qualityId = rock_->qualityId;
	auto target = rock_->scene->flyTargets[typeId];
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
	scaleStep = (50 * 1.5f / 128.f - scene->cRocksScale) / moveCount;
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
	auto& f = gg.tf.rocks_[typeId][4];
	gg.Quad().Draw(f, f, scene_->cam.ToGLPos(pos), 0.5f, scale * scene_->cam.scale);
}
