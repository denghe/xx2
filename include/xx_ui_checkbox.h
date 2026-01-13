#pragma once
#include "xx_ui_label.h"
#include "xx_ui_button.h"

namespace xx {

	struct CheckBox : Button {
		static constexpr int32_t cTypeId{ 17 };
		TinyFrame icon0, icon1;
		bool value{};
		std::function<void(int32_t)> onValueChanged = [](bool v) { printf("CheckBox value = %d", v); };

		CheckBox& Init(int32_t z_, XY position_, XY anchor_, XY fixedSize_, bool value_
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH
			, TinyFrame icon0_ = GameBase::instance->embed.ui_checkbox_0
			, TinyFrame icon1_ = GameBase::instance->embed.ui_checkbox_1
		);

		template<typename S>
		CheckBox& operator()(S const& txt_) {
			At<Label>(0)(txt_);
			return *this;
		}

		void ApplyCfg() override;
	};

}
