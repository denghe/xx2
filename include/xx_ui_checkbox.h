#pragma once
#include "xx_ui_label.h"
#include "xx_ui_imagebutton.h"

namespace xx {

	struct CheckBox : Button {
		static constexpr int32_t cTypeId{ 17 };
		TinyFrame icon0, icon1;
		bool value{};
		std::function<void(int32_t)> onValueChanged = [](bool v) { CoutN("CheckBox value = ", v); };

		CheckBox& Init(int32_t z_, XY position_, XY anchor_, XY fixedSize_, bool value_
			, Ref<Scale9Config> cfgNormal_ = GameBase_shader::GetInstance()->embed.cfg_s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_shader::GetInstance()->embed.cfg_s9bH
			, TinyFrame icon0_ = GameBase_shader::GetInstance()->embed.ui_checkbox_0
			, TinyFrame icon1_ = GameBase_shader::GetInstance()->embed.ui_checkbox_1
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

		template<typename S>
		CheckBox& operator()(S const& txt_) {
			At<Label>(0)(txt_);
			return *this;
		}

		void ApplyCfg() override {
			At<Scale9>(2).Init(z, 0, 0, size, GetCfg());
		}
	};

}
