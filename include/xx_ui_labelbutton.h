#pragma once
#include "xx_ui_label.h"
#include "xx_ui_button.h"

namespace xx {

	struct LabelButton : Button {
		static constexpr int32_t cTypeId{ 11 };

		XY fixedSize{};

		// step1
		LabelButton& Init(int32_t z_, XY position_, XY anchor_
			, float fontSize_ = 0, XY fixedSize_ = {}
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH
		);

		// step2
		template<typename S>
		LabelButton& operator()(S const& txt_ = {}) {
			At<Label>(0)(txt_);
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override;
	};

}
