#pragma once
#include "xx_ui_button.h"

namespace xx {

	struct ImageButton : Button {
		static constexpr int32_t cTypeId{ 12 };
		float borderWidth{};

		ImageButton& Init(int32_t z_, XY position_, XY anchor_, float borderWidth_
			, TinyFrame frame_
			, XY fixedSize_ = 0
			, bool keepAspect_ = true
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH
		);

		void ApplyCfg() override;
	};

}
