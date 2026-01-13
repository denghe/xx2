#pragma once
#include "xx_ui_button.h"
#include "xx_gamebase.h"

namespace xx {

	Shared<Scale9Config> const& InteractionNode::GetCfg() const {
		if (selected || focused) {
			return cfgHighlight;
		}
		return cfgNormal;
	}

	void InteractionNode::SetFocus() {
		assert(!focused);
		focused = true;
		SetToUIHandler(true);
		ApplyCfg();
		onFocus();
		//CoutN("SetFocus");
	}

	void InteractionNode::LostFocus() {
		if (!focused) return;
		SetToUIHandler(false);
		focused = false;
		ApplyCfg();
		//CoutN("LostFocus");
	}


	Button& Button::Init(int32_t z_, XY position_, XY anchor_, XY size_
		, Shared<Scale9Config> cfgNormal_
		, Shared<Scale9Config> cfgHighlight_) {
		//assert(typeId == cTypeId);
		z = z_;
		position = position_;
		anchor = anchor_;
		size = size_;
		cfgNormal = std::move(cfgNormal_);
		cfgHighlight = std::move(cfgHighlight_);
		FillTrans();
		auto& cfg = GetCfg();
		Make<Scale9>()->Init(z, 0, 0, size, cfg);
		return *this;
	}

	void Button::ApplyCfg() {
		At<Scale9>(0).Init(z, 0, 0, size, GetCfg());
	}

	// todo: enable disable

	int32_t Button::OnMouseDown(int32_t btnId_) {
		if (btnId_ != GLFW_MOUSE_BUTTON_LEFT
			&& btnId_ != GLFW_MOUSE_BUTTON_RIGHT) return 0;
		if (!enabled) return 0;
		LostFocus();
		if (btnId_ == GLFW_MOUSE_BUTTON_LEFT) {
			onClicked();
		}
		else {
			onClicked2();
		}
		return 1;
	}

	int32_t Button::OnMouseMove() {
		if (!enabled) return 0;
		if (!focused) {
			SetFocus();
			TryRegisterAutoUpdate();
		}
		return 1;
	}

	int32_t Button::Update() {
		if (GameBase::instance->uiHandler.TryGetPointer() != this || !MousePosInArea()) {
			LostFocus();
			return 1;
		}
		return 0;
	}

}
