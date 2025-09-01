#pragma once
#include "xx_ui_label.h"
#include "xx_ui_button.h"

namespace xx {

	struct LabelButton : Button {
		static constexpr int32_t cTypeId{ 11 };

		XY fixedSize{};

		template<typename S1, typename S2 = char const*>
		LabelButton& Init(int z_, XY position_, XY anchor_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_
			, S1 const& txtLeft_, S2 const& txtRight_ = {}, XY fixedSize_ = {}) {
			assert(children.Empty());
			typeId = cTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			cfgNormal = cfgNormal_ ? std::move(cfgNormal_) : GameBase_ui::GetInstance()->defaultCfg.s9bN;
			cfgHighlight = cfgHighlight_ ? std::move(cfgHighlight_) : GameBase_ui::GetInstance()->defaultCfg.s9bH;
			fixedSize = fixedSize_;

			MakeChildren<Label>()->SetText(txtLeft_);
			if (StrLen(txtRight_)) MakeChildren<Label>()->SetText(txtRight_);
			MakeChildren<Scale9>();
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2 || children.len == 3);		// lblLeft [, lblRight], bg
			auto& cfg = GetCfg();
			auto lblLeft = (Label*)children[0].pointer;
			lblLeft->Init(z + 1, { cfg.txtPadding.x * cfg.txtScale, cfg.txtPaddingRightBottom.y * cfg.txtScale }, {}, cfg.txtScale, cfg.txtColor);
			if (fixedSize.x > 0) {
				size = fixedSize;
			}
			else {
				size = lblLeft->GetScaledSize() + cfg.txtPadding * cfg.txtScale + cfg.txtPaddingRightBottom * cfg.txtScale;
			}
			Scale9* bg;
			if (children.len == 3) {
				((Label*)children[1].pointer)->Init(z + 1, { size.x - cfg.txtPadding.x * cfg.txtScale, cfg.txtPaddingRightBottom.y * cfg.txtScale }, { 1, 0 }, cfg.txtScale, cfg.txtColor);
				bg = (Scale9*)children[2].pointer;
			}
			else {
				bg = (Scale9*)children[1].pointer;
			}
			bg->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			FillTransRecursive();
		}

		Label& LabelLeft() {
			assert(children.len == 2 || children.len == 3);
			return *(Label*)children[0].pointer;
		}

		Label& LabelRight() {
			assert(children.len == 3);
			return *(Label*)children[1].pointer;
		}
	};

}
