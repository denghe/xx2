#pragma once
#include "xx_ui_scale9.h"

namespace xx {

	// for swallow mouse event( outside )
	struct Background : MouseEventHandlerNode {
		static constexpr int32_t cTypeId{ 14 };
		std::function<void()> onOutsideClicked = [] { CoutN("Background Outside clicked."); };
		Weak<Node> content;

		Background& Init(int z_, Weak<Node> contentArea_) {
			Node::InitDerived<Background>(z_, 0, 0.5, 1, GameBase::instance->uiGrid.worldSize);
			content = std::move(contentArea_);
			return *this;
		}

		virtual int32_t OnMouseDown(int32_t btnId_) override {
			if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
			if (content && content->PosInArea(GameBase::instance->mousePos)) {
				// do nothing
			}
			else {
				onOutsideClicked();
			}
			return 1;	// swallow
		}

		virtual int32_t OnMouseMove() override {
			return 1;	// swallow
		}

	};

}
