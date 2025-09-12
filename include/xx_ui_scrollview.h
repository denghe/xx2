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

		void Init(int32_t z_, XY const& position_, XY const& scale_, XY const& anchor_, XY const& size_, XY const& contentSize_) {
			Node::InitDerived<ScrollView>(z_, position_, scale_, anchor_, size_);
			children.Clear();
			auto&& c = Make<Node>();	// children[0] is content node
			c->scissor = WeakFromThis(this);
			CalcDragLimit(contentSize_);
			c->Init(z_, basePos + dragLimit, { 1,1 }, {}, contentSize_);
		}

		template<typename T>
		XX_INLINE Shared<T>& MakeContent() {
			auto& c = children[0];
			assert(c->scissor.pointer() == this);
			auto& r = c->Make<T>();
			r->scissor = WeakFromThis(this);
			return r;
		}

		template<bool resetPosition = true>
		XX_INLINE void InitContentSize(XY const& contentSize_) {
			auto& c = children[0];
			assert(c->scissor.pointer() == this);
			if (c->size != contentSize_) {
				c->size = contentSize_;
				CalcDragLimit(contentSize_);
				if constexpr (resetPosition) {
					c->position = { basePos.x, basePos.y + dragLimit.y };
					c->FillTransRecursive();
				} else {
					UpdateContentPosition(c->position);
				}
			}
		}

		XX_INLINE void CalcDragLimit(XY const& contentSize_) {
			if (size.y > contentSize_.y) {
				basePos = { 0, size.y - contentSize_.y };
			} else {
				basePos = {};
			}
			dragLimit.x = contentSize_.x > size.x ? -(contentSize_.x - size.x) : 0;
			dragLimit.y = contentSize_.y > size.y ? -(contentSize_.y - size.y) : 0;
		}

		XX_INLINE bool UpdateContentPosition(XY pos) {
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

		List<ZNode> tmpZNodes;
		virtual void Draw() override {
			DirectDrawTo(worldMinXY, worldSize, [&] {
				FillZNodes<false>(tmpZNodes, children[0]);
				OrderByZDrawAndClear(tmpZNodes);
				});
		};

		XY mouseDownPos{}, mouseLastPos{};
		virtual int32_t OnMouseDown(int32_t btnId_) override {
			if (btnId_ != GLFW_MOUSE_BUTTON_LEFT) return 0;
			auto& eb = *GameBase::instance;
			assert(!eb.uiHandler);
			eb.uiHandler = WeakFromThis(this);
			mouseLastPos = mouseDownPos = eb.mousePos;
			return 1;
		}

		virtual int32_t OnMouseMove() override {
			auto& eb = *GameBase::instance;
			if (eb.uiHandler.TryGetPointer() == this) {
				auto d = eb.mousePos - mouseLastPos;
				mouseLastPos = eb.mousePos;
				UpdateContentPosition(children[0]->position + d);
			}
		}

	};

}
