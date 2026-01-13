#pragma once
#include "xx_ui_imagelabelbutton.h"
#include "xx_ui_image.h"
#include "xx_gamebase.h"

namespace xx {

	ImageLabelButton& ImageLabelButton::Init(int32_t z_, XY position_, XY anchor_
		, float fontSize_, float marginLeft_, XY fixedSize_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_
	) {
		assert(children.Empty());
		assert(typeId == cTypeId);
		focused = false;
		z = z_;
		position = position_;
		anchor = anchor_;
		fontSize = fontSize_;
		marginLeft = marginLeft_;
		fixedSize = fixedSize_;
		cfgNormal = std::move(cfgNormal_);
		cfgHighlight = std::move(cfgHighlight_);

		auto& cfg = GetCfg();
		if (fixedSize.x > 0 || fixedSize.y > 0) {
			assert(fontSize == 0);
			fontSize = size.y - cfg->paddings.TopBottom();
		}
		else {
			assert(fontSize > 0);
		}
		Make<Label>();
		Make<Image>();
		Make<Scale9>();
		return *this;
	}

	ImageLabelButton& ImageLabelButton::operator()(TinyFrame frame_, XY fixedSize_, XY offset_, bool keepAspect_) {
		At<Image>(1).Init(z + 2, offset_, 0.5f, std::move(frame_), fixedSize_, keepAspect_);
		if (marginLeft == 0) {
			auto& cfg = GetCfg();
			marginLeft = offset_.x + fixedSize_.x * 0.5f + cfg->paddings.top;
		}
		return *this;
	}

	void ImageLabelButton::ApplyCfg() {
		auto& cfg = GetCfg();
		if (fixedSize.x > 0) size = fixedSize;
		else size = At<Label>(0).GetScaledSize() + cfg->paddings.RightTopBottom() + XY{ marginLeft, 0 };
		At<Scale9>(2).Init(z, 0, 0, size, cfg);
		FillTransRecursive();
	}

}
