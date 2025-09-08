#pragma once
#include "xx_ui_imagebutton.h"
#include "xx_ui_label.h"

namespace xx {

	// todo: test

	struct ImageLabelButton : Button {
		static constexpr int32_t cTypeId{ 18 };

		XY fixedSize{};
		float fontSize{}, marginLeft{};

		// step 1
		ImageLabelButton& Init(int z_, XY position_, XY anchor_
			, float fontSize_ = 0, float marginLeft_ = 0, XY fixedSize_ = {}
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->embed.cfg_s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->embed.cfg_s9bH
		) {
			assert(children.Empty());
			assert(typeId == cTypeId);
			focused = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			fontSize = fontSize_;
			marginLeft = marginLeft_;
			fixedSize = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);

			auto& cfg = GetCfg();
			if (fixedSize.x > 0 || fixedSize.y > 0) {
				assert(fontSize == 0);
				fontSize = size.y - cfg->paddings.TopBottom();
			}
			else {
				assert(fontSize > 0);
			}
			Make<Label>();
			Make<Image>();
			Make<Scale9>();
			return *this;
		}

		// step2: set icon( offset_'s pivot: 0.5 )
		ImageLabelButton& operator()(TinyFrame frame_, XY size_, XY offset_) {
			At<Image>(1).Init(z + 2, offset_, 0.5f, size_, true, std::move(frame_));
			if (marginLeft == 0) {
				auto& cfg = GetCfg();
				marginLeft = offset_.x + size_.x * 0.5f + cfg->paddings.top;
			}
			return *this;
		}

		// step3: set text
		template<typename S>
		ImageLabelButton& operator()(S const& txt_) {
			auto& cfg = GetCfg();
			XY pos{ marginLeft, cfg->paddings.bottom };
			At<Label>(0).Init(z + 1, pos, 0, fontSize)(txt_);
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override {
			auto& cfg = GetCfg();
			if (fixedSize.x > 0) size = fixedSize;
			else size = At<Label>(0).GetScaledSize() + cfg->paddings.RightTopBottom() + XY{ marginLeft, 0 };
			At<Scale9>(2).Init(z, 0, 0, size, cfg);
			FillTransRecursive();
		}
	};

}
