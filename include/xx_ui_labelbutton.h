#pragma once
#include "xx_ui_label.h"
#include "xx_ui_button.h"

namespace xx {

	struct LabelButton : Button {
		static constexpr int32_t cTypeId{ 11 };

		XY fixedSize{};

		LabelButton& Init(int z_, XY position_, XY anchor_
			, XY fixedSize_ = {}
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->defaultCfg.s9bH
		) {
			assert(children.Empty());
			typeId = cTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			fixedSize = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);

			MakeChildren<Label>();
			MakeChildren<Scale9>();
			return *this;
		}

		// for easy use
		template<typename S>
		LabelButton& operator()(S const& txt_ = {}, float maxWidth_ = 0) {
			RefLabel().SetText(txt_, maxWidth_);
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2);
			auto& cfg = GetCfg();
			auto lblLeft = (Label*)children[0].pointer;
			lblLeft->Init(z + 1, { cfg.txtPadding.x * cfg.txtScale, cfg.txtPaddingRightBottom.y * cfg.txtScale }, {}, cfg.txtScale, cfg.txtColor);
			if (fixedSize.x > 0) {
				size = fixedSize;
			}
			else {
				size = lblLeft->GetScaledSize() + cfg.txtPadding * cfg.txtScale + cfg.txtPaddingRightBottom * cfg.txtScale;
			}
			auto bg = (Scale9*)children[1].pointer;
			bg->Init(z, {}, {}, cfg.borderScale, size / cfg.borderScale, cfg);
			FillTransRecursive();
		}

		// for easy use( need call ApplyCfg() after change label content )
		Label& RefLabel() {
			assert(children.len == 2);
			return *(Label*)children[0].pointer;
		}
	};

}
