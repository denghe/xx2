#pragma once
#include "xx_ui_slider.h"
#include "xx_gamebase.h"
#include "xx_glfw.h"

namespace xx {

	Slider& Slider::Init(int32_t z_, XY position_, XY anchor_
		, float height_
		, float widthTxtLeft_, float widthBar_, float widthTxtRight_
		, double value_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_
		, Shared<Scale9Config> cfgBar_
		, Shared<Scale9Config> cfgBlock_
	)
	{
		assert(children.Empty());
		assert(typeId == cTypeId);
		focused = false;
		z = z_;
		position = position_;
		anchor = anchor_;
		cfgNormal = std::move(cfgNormal_);
		cfgHighlight = std::move(cfgHighlight_);
		cfgBar = std::move(cfgBar_);
		cfgBlock = std::move(cfgBlock_);
		height = height_;
		widthTxtLeft = widthTxtLeft_;
		widthBar = widthBar_;
		widthTxtRight = widthTxtRight_;
		blockMoving = false;
		value = valueBak = value_;
		assert(value >= 0 && value <= 1);
		size = { widthTxtLeft + widthBar + widthTxtRight, height };
		FillTrans();

		auto& cfg = GetCfg();
		auto fontSize = size.y - cfg->paddings.TopBottom();
		Make<Label>()->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize);
		Make<Scale9>();
		Make<Scale9>();
		Make<Label>()->Init(z + 1, { size.x - cfg->paddings.right,  cfg->paddings.bottom }, { 1, 0 }, fontSize);
		Make<Scale9>();
		ApplyValue();
		ApplyCfg();
		return *this;
	}

	void Slider::ApplyCfg() {
		At<Scale9>(4).Init(z, 0, 0, size, GetCfg());
	}

	Slider& Slider::SetValue(double v) {
		value = valueBak = v;
		return *this;
	}

	void Slider::ApplyValue() {
		assert(value >= 0 && value <= 1);

		auto& cfg = GetCfg();

		auto barMinWidth = float(cfgBar->center.x + cfgBar->center.w) * cfgBar->textureScale.x + 1;
		XY barSize{ std::max(widthBar * (float)value, barMinWidth), (height - cfg->paddings.TopBottom()) * 0.33f };
		XY barPos{ widthTxtLeft, (height - barSize.y) * 0.5f };
		At<Scale9>(1).Init(z + 1, barPos, 0, barSize, cfgBar);

		XY blockSize{ barSize.y * 2.f };
		XY blockPos{ widthTxtLeft + widthBar * value - blockSize.x * 0.5f, (height - blockSize.y) * 0.5f };
		At<Scale9>(2).Init(z + 2, blockPos, 0, blockSize, cfgBlock);

		auto txtRight = valueToString(value);
		At<Label>(3)(txtRight);
	}

	void Slider::DragEnd() {
		if (!blockMoving) return;
		blockMoving = false;
		if (valueBak != value) {
			onChanged(value);
			valueBak = value;
		}
	}

	int32_t Slider::OnMouseDown(int32_t btnId_) {
		if (!enabled) return 0;
		if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
		auto mx = GameBase::instance->mousePos.x;
		auto x1 = worldMinXY.x + widthTxtLeft * worldScale.x;
		auto x2 = worldMinXY.x + (widthTxtLeft + widthBar) * worldScale.x;
		assert(worldMaxXY.x > x1);
		assert(worldMaxXY.x > x2);
		if (mx <= x1) {
			value = 0;
		}
		else if (mx >= x2) {
			value = 1;
		}
		else {
			value = (mx - x1) / (x2 - x1);
		}
		ApplyValue();
		blockMoving = true;
		return 1;
	}

	int32_t Slider::OnMouseMove() {
		if (!enabled) return 0;
		if (!focused) {
			SetFocus();
			TryRegisterAutoUpdate();
		}
		if (blockMoving) {
			OnMouseDown(GLFW_MOUSE_BUTTON_LEFT);
		}
		return 1;
	}

	int32_t Slider::Update() {
		auto g = GameBase::instance;
		if (g->uiHandler.TryGetPointer() != this || !MousePosInArea()) {
			LostFocus();
			DragEnd();
			return 1;
		}
		if (!g->mouse[GLFW_MOUSE_BUTTON_LEFT]) {
			DragEnd();
		} else if (callbackWhenBlockMoving && blockMoving && valueBak != value) {
			onChanged(value);
			valueBak = value;
			ApplyValue();
		}
		return 0;
	}

}
