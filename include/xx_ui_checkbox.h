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
			typeId = cTypeId;
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
			auto lblLeft = MakeChildren<Label>();
			lblLeft->Init(z + 1, { cfg.txtPadding.x * cfg.txtScale, cfg.txtPaddingRightBottom.y * cfg.txtScale }, {}, cfg.txtScale, cfg.txtColor);
			auto imgSize = size.y * 0.8f;
			auto imgSpacing = (size.y - imgSize) * 0.5f;
			XY imgPos{ size.x - imgSpacing, imgSpacing };
			MakeChildren<Image>()->Init(z + 2, imgPos, { 1, 0 }, imgSize, true, value ? icon1 : icon0);
			MakeChildren<Scale9>()->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);

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
			assert(children.len >= 3);
			auto& cfg = GetCfg();
			assert(children[2]->typeId == Scale9::cTypeId);
			auto bg = (Scale9*)children[2].pointer;
			bg->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}

		Label& RefLabel() {
			assert(children.len == 3);
			return *(Label*)children[0].pointer;
		}
	};

}
