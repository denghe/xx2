#pragma once
#include "xx_ui_checkbox.h"
#include "xx_ui_image.h"
#include "xx_gamebase.h"

namespace xx {

	CheckBox& CheckBox::Init(int32_t z_, XY position_, XY anchor_, XY fixedSize_, bool value_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_
		, TinyFrame icon0_
		, TinyFrame icon1_
	) {
		assert(typeId == cTypeId);
		z = z_;
		position = position_;
		anchor = anchor_;
		size = fixedSize_;
		cfgNormal = std::move(cfgNormal_);
		cfgHighlight = std::move(cfgHighlight_);
		icon0 = std::move(icon0_);
		icon1 = std::move(icon1_);
		value = value_;
		FillTrans();

		auto& cfg = GetCfg();
		auto fontSize = size.y - cfg->paddings.TopBottom();
		Make<Label>()->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize);
		auto imgSize = XY{ size.y - cfg->paddings.TopBottom() };
		assert(imgSize.x > 0 && imgSize.y > 0);
		Make<Image>()->Init(z + 2, { size.x - cfg->paddings.right, cfg->paddings.bottom }, { 1, 0 }, value ? icon1 : icon0, imgSize);
		Make<Scale9>()->Init(z, 0, 0, size, cfg);

		onClicked = [this] {
			if (!enabled) return;
			value = !value;
			At<Image>(1).frame = value ? icon1 : icon0;
			onValueChanged(value);
		};
		return *this;
	}

	void CheckBox::ApplyCfg() {
		At<Scale9>(2).Init(z, 0, 0, size, GetCfg());
	}

}
