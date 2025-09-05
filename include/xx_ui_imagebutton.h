#pragma once
#include "xx_ui_button.h"
#include "xx_ui_image.h"

namespace xx {

	struct ImageButton : Button {
		static constexpr int32_t cTypeId{ 12 };
		float borderWidth{};

		ImageButton& Init(int z_, XY position_, XY anchor_, XY fixedSize_, float borderWidth_
			, TinyFrame frame_
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->defaultCfg.s9bH
		) {
			assert(typeId == cTypeId);
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			borderWidth = borderWidth_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			FillTrans();
			MakeChildren<Image>()->Init(z_ + 1, 0, 0, fixedSize_, true, std::move(frame_));
			MakeChildren<Scale9>();
			ApplyCfg();
			return *this;
		}

		void ApplyCfg() override {
			auto& cfg = GetCfg();
			At<Scale9>(1).Init(z, -borderWidth, 0, size + borderWidth * 2, GetCfg());
		}
	};

}
