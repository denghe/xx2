#pragma once
#include "xx_ui_imagebutton.h"
#include "xx_ui_label.h"

namespace xx {

	struct ImageLabelButton : Button {
		static constexpr int32_t cTypeId{ 18 };

		XY fixedSize{};
		float fontSize{}, marginLeft{};

		// step 1
		ImageLabelButton& Init(int32_t z_, XY position_, XY anchor_
			, float fontSize_ = 0, float marginLeft_ = 0, XY fixedSize_ = {}
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH
		);

		// step2: set icon( offset_'s pivot: 0.5 )
		ImageLabelButton& operator()(TinyFrame frame_, XY fixedSize_, XY offset_, bool keepAspect_ = true);

		// step3: set text
		template<typename S>
		ImageLabelButton& operator()(S const& txt_) {
			auto& cfg = GetCfg();
			XY pos{ marginLeft, cfg->paddings.bottom };
			At<Label>(0).Init(z + 1, pos, 0, fontSize)(txt_);
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override;
	};

}
