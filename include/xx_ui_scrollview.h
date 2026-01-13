#pragma once
#include "xx_node.h"
#include "xx_scissor.h"

namespace xx {

	// todo: test
	// todo: mouse wheel
	// todo: register delay func scan mouse btn up
	//virtual void OnMouseUp(int32_t btnId_) override {
	//	if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return;
	//	auto& eb = *GameBase::instance;
	//	assert(eb.uiHandler.pointer() == this);
	//	eb.uiHandler.Reset();
	//}

	// content align: left-top
	struct ScrollView : MouseEventHandlerNode, Scissor {

		XY basePos{}, dragLimit{};

		void Init(int32_t z_, XY const& position_, XY const& scale_, XY const& anchor_, XY const& size_, XY const& contentSize_);

		template<typename T>
		Shared<T>& MakeContent() {
			auto& c = children[0];
			assert(c->scissor.pointer() == this);
			auto& r = c->Make<T>();
			r->scissor = WeakFromThis(this);
			return r;
		}

		void InitContentSize(XY const& contentSize_, bool resetPosition_ = true);
		void CalcDragLimit(XY const& contentSize_);
		bool UpdateContentPosition(XY pos);

		List<ZNode> tmpZNodes;
		virtual void Draw() override;

		XY mouseDownPos{}, mouseLastPos{};
		virtual int32_t OnMouseDown(int32_t btnId_) override;
		virtual int32_t OnMouseMove() override;

	};

}
