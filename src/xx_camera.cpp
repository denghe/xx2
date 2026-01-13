#include "xx_camera.h"

namespace xx {

	void Camera::Init(float baseScale_, float logicScale_, XY original_) {
		baseScale = baseScale_;
		logicScale = logicScale_;
		scale = logicScale * baseScale;
		_1_scale = 1.f / scale;
		original = original_;
	}

	void Camera::SetOriginal(XY original_) {
		original = original_;
	}

	void Camera::SetBaseScale(float baseScale_) {
		baseScale = baseScale_;
		scale = logicScale * baseScale;
		_1_scale = 1.f / scale;
	}

	void Camera::SetLogicScale(float logicScale_) {
		logicScale = logicScale_;
		scale = logicScale * baseScale;
		_1_scale = 1.f / scale;
	}

	XY Camera::ToGLPos(XY logicPos_) const {
		return (logicPos_ - original - offset).FlipY() * scale;
	}

	XY Camera::ToLogicPos(XY glPos_) const {
		return { glPos_.x * _1_scale + original.x + offset.x, -glPos_.y * _1_scale + original.y + offset.y };
	}

}
