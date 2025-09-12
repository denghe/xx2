#pragma once
#include "xx_ui_button.h"
#include "xx_ui_image.h"

namespace xx {

	struct ImageButton : Button {
		static constexpr int32_t cTypeId{ 12 };
		float borderWidth{};

		ImageButton& Init(int32_t z_, XY position_, XY anchor_, float borderWidth_
			, TinyFrame frame_
			, XY fixedSize_ = 0
			, bool keepAspect_ = true
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->embed.cfg_s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->embed.cfg_s9bH
		) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			if (fixedSize_.IsZeroSimple()) {
				fixedSize_ = { frame_.textureRect.w, frame_.textureRect.h };
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

		void ApplyCfg() override {
			auto& cfg = GetCfg();
			At<Scale9>(1).Init(z, -borderWidth, 0, size + borderWidth * 2, GetCfg());
		}
	};

}
