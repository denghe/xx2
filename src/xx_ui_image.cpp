#pragma once
#include "xx_ui_image.h"
#include "xx_gamebase.h"

namespace xx {

	Image& Image::Init(int32_t z_, XY position_, XY anchor_
		, TinyFrame frame_
		, XY fixedSize_
		, bool keepAspect_
		, ImageRadians radians_
		, RGBA8 color_) {
		assert(typeId == cTypeId);
		z = z_;
		position = position_;
		anchor = anchor_;
		if (fixedSize_.IsZeroSimple()) {
			fixedSize_ = { frame_.uvRect.w, frame_.uvRect.h };
		}
		size = fixedSize_;
		frame = std::move(frame_);
		radians = float(M_PI) * 0.5f * (int32_t)radians_;
		color = color_;

		if (keepAspect_) {
			auto s = fixedSize_.x / frame.uvRect.w;
			if (frame.uvRect.h * s > fixedSize_.y) {
				s = fixedSize_.y / frame.uvRect.h;
			}
			fixedScale = s;
		}
		else {
			fixedScale.x = fixedSize_.x / frame.uvRect.w;
			fixedScale.y = fixedSize_.y / frame.uvRect.h;
		}

		FillTrans();
		return *this;
	}

	void Image::Draw() {
		if (!frame.tex) return;
		float cp;
		if (enabled) cp = 1.f;
		else cp = 0.5f;
		GameBase::instance->Quad().Draw(*frame.tex, frame.uvRect, worldMinXY, 0
			, worldScale * fixedScale, radians, cp, { color.r, color.g, color.b, (uint8_t)(color.a * alpha) });
	}

}
