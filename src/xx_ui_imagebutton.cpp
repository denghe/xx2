#pragma once
#include "xx_ui_imagebutton.h"
#include "xx_ui_image.h"
#include "xx_gamebase.h"

namespace xx {

	ImageButton& ImageButton::Init(int32_t z_, XY position_, XY anchor_, float borderWidth_
		, TinyFrame frame_
		, XY fixedSize_
		, bool keepAspect_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_
	) {
		assert(typeId == cTypeId);
		z = z_;
		position = position_;
		anchor = anchor_;
		if (fixedSize_.IsZeroSimple()) {
			fixedSize_ = { frame_.uvRect.w, frame_.uvRect.h };
		}
		size = fixedSize_;
		borderWidth = borderWidth_;
		cfgNormal = std::move(cfgNormal_);
		cfgHighlight = std::move(cfgHighlight_);
		FillTrans();
		Make<Image>()->Init(z_ + 1, 0, 0, std::move(frame_), fixedSize_, keepAspect_);
		Make<Scale9>();
		ApplyCfg();
		return *this;
	}

	void ImageButton::ApplyCfg() {
		auto& cfg = GetCfg();
		At<Scale9>(1).Init(z, -borderWidth, 0, size + borderWidth * 2, GetCfg());
	}

}
