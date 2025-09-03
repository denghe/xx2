#pragma once
#include "xx_ui_button.h"
#include "xx_ui_image.h"

namespace xx {

	struct ImageButton : Button {
		static constexpr int32_t cTypeId{ 12 };

		ImageButton& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool keepAspect_
			, TinyFrame frame_
			, ImageRadians radians_ = ImageRadians::Zero
			, RGBA8 color_ = RGBA8_White
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->defaultCfg.s9bH
		) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			MakeChildren<Image>();
			MakeChildren<Scale9>();
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override {
			auto& cfg = GetCfg();
			At<Scale9>(1).Init(z + 1, -cfg->paddings.LeftBottom(), 0, size + cfg->paddings.Total(), GetCfg());
		}
	};

}
