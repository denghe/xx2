#pragma once
#include "xx_ui_labelbutton.h"
#include "xx_gamebase.h"

namespace xx {

	// step1
	LabelButton& LabelButton::Init(int32_t z_, XY position_, XY anchor_
		, float fontSize_, XY fixedSize_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_
	) {
		assert(children.Empty());
		assert(typeId == cTypeId);
		focused = false;
		z = z_;
		position = position_;
		anchor = anchor_;
		fixedSize = fixedSize_;
		cfgNormal = std::move(cfgNormal_);
		cfgHighlight = std::move(cfgHighlight_);

		auto& cfg = GetCfg();
		if (fixedSize_.x > 0 || fixedSize_.y > 0) {
			assert(fontSize_ == 0);
			fontSize_ = size.y - cfg->paddings.TopBottom();
		}
		else {
			assert(fontSize_ > 0);
		}
		Make<Label>()->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize_);
		Make<Scale9>();
		return *this;
	}

	void LabelButton::ApplyCfg() {
		assert(children.len == 2);
		auto& cfg = GetCfg();
		if (fixedSize.x > 0) size = fixedSize;
		else size = At<Label>(0).GetScaledSize() + cfg->paddings.Total();
		At<Scale9>(1).Init(z, 0, 0, size, cfg);
		FillTransRecursive();
	}

}
