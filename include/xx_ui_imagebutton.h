#pragma once
#include "xx_ui_button.h"
#include "xx_ui_image.h"

namespace xx {

	struct ImageButton : Button {
		static constexpr int32_t cTypeId{ 12 };

		ImageButton& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_, XY spacing_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_
			, TinyFrame frame_, ImageRadians radians_ = ImageRadians::Zero, RGBA8 color_ = RGBA8_White, float colorplus_ = 1) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_ + spacing_ * 2;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			MakeChildren<Image>()->Init(z, spacing_, {}, fixedSize_, keepAspect_, std::move(frame_), radians_, color_, colorplus_);
			auto& cfg = GetCfg();
			MakeChildren<Scale9>()->Init(z + 1, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2);
			auto& cfg = GetCfg();
			auto bg = (Scale9*)children[1].pointer;
			bg->Init(z + 1, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}
	};

}
