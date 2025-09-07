#pragma once
#include "xx_ui_label.h"
#include "xx_ui_button.h"

namespace xx {

	struct LabelButton : Button {
		static constexpr int32_t cTypeId{ 11 };

		XY fixedSize{};

		LabelButton& Init(int z_, XY position_, XY anchor_
			, float fontSize_ = 0, XY fixedSize_ = {}
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->defaultCfg.s9bH
		) {
			assert(children.Empty());
			assert(typeId == cTypeId);
			focused = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			fixedSize = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);

			auto& cfg = GetCfg();
			if (fixedSize_.x > 0 || fixedSize_.y > 0) {
				assert(fontSize_ == 0);
				fontSize_ = size.y - cfg->paddings.TopBottom();
			}
			else {
				assert(fontSize_ > 0);
			}
			Make<Label>()->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize_);
			Make<Scale9>();
			return *this;
		}

		// for easy use
		template<typename S>
		LabelButton& operator()(S const& txt_ = {}) {
			At<Label>(0)(txt_);
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override {
			assert(children.len == 2);
			auto& cfg = GetCfg();
			auto& label = At<Label>(0);
			
			if (fixedSize.x > 0) size = fixedSize;
			else size = label.GetScaledSize() + cfg->paddings.Total();
			At<Scale9>(1).Init(z, 0, 0, size, cfg);
			FillTransRecursive();
		}
	};

}
