#pragma once
#include "xx_ui_label.h"
#include "xx_ui_imagebutton.h"

namespace xx {

	struct CheckBox : Button {
		static constexpr int32_t cTypeId{ 16 };
		TinyFrame icon0, icon1;
		bool value{};
		std::function<void(int32_t)> onValueChanged = [](bool v) { CoutN("CheckBox value = ", v); };

		CheckBox& Init(int z_, XY position_, XY anchor_, XY fixedSize_, bool value_
			, Ref<Scale9Config> cfgNormal_ = GameBase_shader::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_shader::GetInstance()->defaultCfg.s9bH
			, TinyFrame icon0_ = GameBase_shader::GetInstance()->defaultRes.ui_checkbox_0
			, TinyFrame icon1_ = GameBase_shader::GetInstance()->defaultRes.ui_checkbox_1
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
			MakeChildren<Label>()->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize);
			auto imgSize = XY{ size.y - cfg->paddings.TopBottom() };
			assert(imgSize.x > 0 && imgSize.y > 0);
			MakeChildren<Image>()->Init(z + 2, { size.x - cfg->paddings.right, cfg->paddings.bottom }, { 1, 0 }, imgSize, true, value ? icon1 : icon0);
			MakeChildren<Scale9>()->Init(z, 0, 0, size, cfg);

			onClicked = [this] {
				value = !value;
				assert(children[1]->typeId == Image::cTypeId);
				((Image*)children[1].pointer)->frame = value ? icon1 : icon0;
				onValueChanged(value);
			};
			return *this;
		}

		template<typename S>
		CheckBox& operator()(S const& txt_) {
			RefLabel().SetText(txt_);
			return *this;
		}

		void ApplyCfg() override {
			At<Scale9>(2).Init(z, 0, 0, size, GetCfg());
		}

		Label& RefLabel() {
			assert(children.len == 3);
			return *(Label*)children[0].pointer;
		}
	};

}
