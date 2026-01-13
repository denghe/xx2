#pragma once
#include "xx_ui_label.h"
#include "xx_ui_scale9.h"
#include "xx_ui_button.h"

namespace xx {

	struct Slider : InteractionNode {
		static constexpr int32_t cTypeId{ 13 };

		bool callbackWhenBlockMoving{};	// user can set directly
		Shared<Scale9Config> cfgBar, cfgBlock;
		float height{}, widthTxtLeft{}, widthBar{}, widthTxtRight{};
		double value{}, valueBak{};	// 0 ~ 1
		bool blockMoving{};

		std::function<void(double)> onChanged = [](double v) { printf("Slider changed. v = %f\n", v); };
		std::function<std::string(double)> valueToString = [](double v)->std::string {
			return std::to_string(int32_t(v * 100));
		};

		// InitBegin + set value/ToSting + InitEnd
		Slider& Init(int32_t z_, XY position_, XY anchor_
			, float height_
			, float widthTxtLeft_, float widthBar_, float widthTxtRight_
			, double value_
			, Shared<Scale9Config> cfgNormal_ = GameBase::instance->embed.cfg_s9bN
			, Shared<Scale9Config> cfgHighlight_ = GameBase::instance->embed.cfg_s9bH
			, Shared<Scale9Config> cfgBar_ = GameBase::instance->embed.cfg_sbar
			, Shared<Scale9Config> cfgBlock_ = GameBase::instance->embed.cfg_sblock
		);

		template<typename S>
		Slider& operator()(S const& txtLeft_) {
			At<Label>(0)(txtLeft_);
			return *this;
		}

		void ApplyCfg() override;
		Slider& SetValue(double v);
		void ApplyValue();
		void DragEnd();
		// todo: enable disable
		virtual int32_t OnMouseDown(int32_t btnId_) override;
		int32_t OnMouseMove() override;
		int32_t Update() override;

	};
}
