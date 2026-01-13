#pragma once
#include "xx_ui_scrollview.h"
#include "xx_gamebase.h"

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

	void ScrollView::Init(int32_t z_, XY const& position_, XY const& scale_, XY const& anchor_, XY const& size_, XY const& contentSize_) {
		Node::InitDerived<ScrollView>(z_, position_, scale_, anchor_, size_);
		children.Clear();
		auto&& c = Make<Node>();	// children[0] is content node
		c->scissor = WeakFromThis(this);
		CalcDragLimit(contentSize_);
		c->Init(z_, basePos + dragLimit, { 1,1 }, {}, contentSize_);
	}

	void ScrollView::InitContentSize(XY const& contentSize_, bool resetPosition_) {
		auto& c = children[0];
		assert(c->scissor.pointer() == this);
		if (c->size != contentSize_) {
			c->size = contentSize_;
			CalcDragLimit(contentSize_);
			if (resetPosition_) {
				c->position = { basePos.x, basePos.y + dragLimit.y };
				c->FillTransRecursive();
			}
			else {
				UpdateContentPosition(c->position);
			}
		}
	}

	void ScrollView::CalcDragLimit(XY const& contentSize_) {
		if (size.y > contentSize_.y) {
			basePos = { 0, size.y - contentSize_.y };
		} else {
			basePos = {};
		}
		dragLimit.x = contentSize_.x > size.x ? -(contentSize_.x - size.x) : 0;
		dragLimit.y = contentSize_.y > size.y ? -(contentSize_.y - size.y) : 0;
	}

	bool ScrollView::UpdateContentPosition(XY pos) {
		auto& c = children[0];
		assert(c->scissor.pointer() == this);
		if (pos.x > 0) pos.x = 0;
		else if (pos.x < dragLimit.x) pos.x = dragLimit.x;
		if (pos.y > basePos.y) pos.y = basePos.y;
		else if (pos.y < basePos.y + dragLimit.y) pos.y = basePos.y + dragLimit.y;
		if (pos != c->position) {
			c->position = pos;
			c->FillTransRecursive();
			//CoutN(pos);
			return true;
		}
		return false;
	}

	void ScrollView::Draw() {
		DirectDrawTo(worldMinXY, worldSize, [&] {
			FillZNodes(tmpZNodes, children[0], false);
			OrderByZDrawAndClear(tmpZNodes);
		});
	};

	int32_t ScrollView::OnMouseDown(int32_t btnId_) {
		if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
		auto& eb = *GameBase::instance;
		assert(!eb.uiHandler);
		eb.uiHandler = WeakFromThis(this);
		mouseLastPos = mouseDownPos = eb.mousePos;
		return 1;
	}

	int32_t ScrollView::OnMouseMove() {
		auto& eb = *GameBase::instance;
		if (eb.uiHandler.TryGetPointer() == this) {
			auto d = eb.mousePos - mouseLastPos;
			mouseLastPos = eb.mousePos;
			UpdateContentPosition(children[0]->position + d);
		}
		// todo
		return 0;
	}

}
