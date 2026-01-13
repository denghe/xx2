#pragma once
#include "xx_ui_background.h"
#include "xx_gamebase.h"

namespace xx {

	Background& Background::Init(int32_t z_, Weak<Node> contentArea_) {
		escHandler = true;
		Node::InitDerived<Background>(z_, 0, 0.5, 1, GameBase::instance->uiGrid.worldSize);
		content = std::move(contentArea_);
		return *this;
	}

	int32_t Background::OnMouseDown(int32_t btnId_) {
		if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
		if (content && content->PosInArea(GameBase::instance->mousePos)) {
			// do nothing
		}
		else {
			onOutsideClicked();
		}
		return 1;	// swallow
	}

	int32_t Background::OnMouseMove() {
		return 1;	// swallow
	}

	void Background::HandleESC() {
		onOutsideClicked();
	}

}
