#include "pch.h"
#include "g2.h"

namespace G2 {

	void FlyingWood::Init(Box* owner_, SceneItem* tar_) {
		typeId = cTypeId;
		owner = owner_;
		scene = tar_->scene;
		pos = tar_->pos;
		y = pos.y;
		radius = tar_->radius;
		radians = tar_->radians;
		scale = tar_->scale;
		indexAtContainer = owner_->flyingWoods.len - 1;
		assert(owner_->flyingWoods[indexAtContainer].pointer == this);

		auto d = owner_->pos - tar_->pos;
		auto mag2 = d.x * d.x + d.y * d.y;
		if (mag2 > 0.0001f) {
			auto mag = std::sqrtf(mag2);
			auto norm = d / mag;
			inc = norm * cFlyingWoodSpeed;
			numSteps = int32_t(mag / cFlyingWoodSpeed);
		}
	}

	bool FlyingWood::Update() {
		if (--numSteps >= 0) {
			pos += inc;
			y = pos.y;
			--numSteps;
			return false;
		}
		return true;
	}

	void FlyingWood::Draw() {
		gg.Quad().DrawFrame(gg.pics.wood2, scene->cam.ToGLPos(pos), scale * scene->cam.scale);
	}

	void FlyingWood::Dispose() {
		auto i = indexAtContainer;
		assert(owner->flyingWoods[i].pointer == this);
		owner->flyingWoods.Back()->indexAtContainer = i;
		indexAtContainer = -1;
		owner->flyingWoods.SwapRemoveAt(i);
	}

}
