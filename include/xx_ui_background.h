#pragma once
#include "xx_ui_scale9.h"

namespace xx {

	// for swallow mouse event( outside )
	struct Background : MouseEventHandlerNode {
		static constexpr int32_t cTypeId{ 14 };
		std::function<void()> onOutsideClicked = [] { printf("Background Outside clicked."); };
		Weak<Node> content;

		Background& Init(int32_t z_, Weak<Node> contentArea_);
		virtual int32_t OnMouseDown(int32_t btnId_) override;
		virtual int32_t OnMouseMove() override;
		virtual void HandleESC() override;
	};

}
