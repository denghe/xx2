#include "xx_node.h"
#include "xx_gamebase.h"
#include "xx_math.h"

namespace xx {

	XY Node::ToLocalPos(XY worldPos_) {
		return trans().MakeInvert()(worldPos_);
	}
	XY Node::ToParentLocalPos(XY worldPos_) {
		return parent->trans().MakeInvert()(worldPos_);
	}

	XY Node::GetScaledSize() const {
		return scale * size;
	}

	void Node::FillTrans() {
		if (parent) {
			trans() = SimpleAffineTransform::MakePosScaleAnchorSize(position, scale, anchor * size).MakeConcat(parent->trans());
		}
		else {
			trans().PosScaleAnchorSize(position, scale, anchor * size);
		}

		worldMaxXY = trans()(size);
		worldSize = worldMaxXY - worldMinXY;

		TransUpdate();
	}

	bool Node::IsVisible() const {
		if (!visible) return false;
		if (scissor && !IsIntersect_BoxBoxF(worldMinXY, worldMaxXY, scissor->worldMinXY, scissor->worldMaxXY)) return false;
		if (inParentArea && parent) return IsIntersect_BoxBoxF(worldMinXY, worldMaxXY, parent->worldMinXY, parent->worldMaxXY);
		return IsIntersect_BoxBoxF(worldMinXY, worldMaxXY, GameBase::instance->worldMinXY, GameBase::instance->worldMaxXY);
	}

	bool Node::PosInArea(XY pos_) const {
		if (scissor && !IsIntersect_BoxPointF(scissor->worldMinXY, scissor->worldMaxXY, pos_)) return false;
		return IsIntersect_BoxPointF(worldMinXY, worldMaxXY, pos_);
	}

	bool Node::PosInScissor(XY pos_) const {
		if (!scissor) return true;
		return IsIntersect_BoxPointF(scissor->worldMinXY, scissor->worldMaxXY, pos_);
	}

	// for update
	void Node::FillTransRecursive() {
		FillTrans();
		for (auto& c : children) {
			c->FillTransRecursive();
		}
	};

	void Node::SetVisibleRecursive(bool visible_) {
		visible = visible_;
		for (auto& c : children) {
			c->SetVisibleRecursive(visible_);
		}
	}

	void Node::SetEnabledRecursive(bool enabled_) {
		enabled = enabled_;
		for (auto& c : children) {
			c->SetEnabledRecursive(enabled_);
		}
	}

	void Node::SetAlphaRecursive(float alpha_) {
		alpha = alpha_;
		for (auto& c : children) {
			c->SetAlphaRecursive(alpha_);
		}
	}

	void Node::SetScissorRecursive(Weak<Node> scissor_) {
		scissor = scissor_;
		for (auto& c : children) {
			c->SetScissorRecursive(scissor_);
		}
	}

	Node& Node::Init(int32_t z_, XY position_, XY anchor_, XY scale_, XY size_) {
		return InitDerived<Node>(z_, position_, anchor_, scale_, size_);
	}

	// for ui root node only
	Node& Node::InitRoot(XY scale_) {
		return Init(0, 0, 0, scale_);
	}

	void Node::SetToUIHandler(bool handle) {
		auto& h = GameBase::instance->uiHandler;
		if (handle) {
			h = WeakFromThis((MouseEventHandlerNode*)this);
		}
		else {
			if ((Node*)h.TryGetPointer() == this) {
				h.Reset();
			}
		}
	}

	void Node::SwapRemove() {
		assert(parent);
		assert(parent->children[indexAtParentChildren].pointer == this);
		auto i = parent->children.Back()->indexAtParentChildren = indexAtParentChildren;
		indexAtParentChildren = -1;
		auto p = parent.GetPointer();
		parent.Reset();
		p->children.SwapRemoveAt(i);
	}

	void Node::Clear() {
		for (auto i = children.len - 1; i >= 0; --i) {
			children[i]->SwapRemove();
		}
	}

	void Node::FindTopESCHandler(Node*& out, int32_t& minZ) {
		if (escHandler && z > minZ) {
			out = this;
			minZ = z;
		}
		for (auto& c : children) {
			c->FindTopESCHandler(out, minZ);
		}
	}

	// recursive find TOP ESC handler
	// null: not found
	Node* Node::FindTopESCHandler() {
		Node* n{};
		auto minZ = std::numeric_limits<int32_t>::min();
		FindTopESCHandler(n, minZ);
		return n;
	}

	void Node::HandleESC() {
		SwapRemove();
	}

	void Node::TryRegisterAutoUpdate() {
		auto& c = GameBase::instance->uiAutoUpdates;
		auto w = WeakFromThis(this);
		for (int32_t i = 0, e = c.len; i < e; ++i) {
			if (c[i].h == w.h) return;
		}
		c.Emplace(std::move(w));
	}


	void Node::Resize(XY scale_) {
		scale = scale_;
		FillTransRecursive();
	}

	void Node::Resize(XY position_, XY scale_) {
		position = position_;
		scale = scale_;
		FillTransRecursive();
	}

	Node* Node::FindFirstTypeId(int32_t typeId_) {
		if (this->typeId == typeId_) return this;
		for (int32_t i = 0; i < children.len; ++i) {
			if (children[i]->typeId == typeId_) {
				return children[i].pointer;
			}
		}
		return {};
	}





	bool MouseEventHandlerNode::MousePosInArea() const {
		return PosInArea(GameBase::instance->mousePos);
	}

	void MouseEventHandlerNode::TransUpdate() {
		auto& g = GameBase::instance->uiGrid;
		auto w2 = g.worldSize * 0.5f;
		FromTo<XY> aabb{ worldMinXY + w2, worldMaxXY + w2 };

		if (g.TryLimitAABB(aabb)) {
			if (indexAtUiGrid != -1) {
				g.Update(indexAtUiGrid, aabb);
			}
			else {
				indexAtUiGrid = g.Add(aabb, this);
			}
		}
		else {
			if (indexAtUiGrid != -1) {
				g.Remove(indexAtUiGrid);
				indexAtUiGrid = -1;
			}
		}
	}

	MouseEventHandlerNode::~MouseEventHandlerNode() {
		if (indexAtUiGrid != -1) {
			GameBase::instance->uiGrid.Remove(indexAtUiGrid);
			indexAtUiGrid = -1;
		}
	}





	void FillZNodes(List<ZNode>& zns, Node* n, bool skipScissorContent) {
		assert(n);
		if (skipScissorContent) {
			if (n->scissor && n->scissor == n->parent) return;
		}
		if ((n->size.x > 0.f || n->size.y > 0.f) && n->IsVisible()) {
			zns.Emplace(n->z, n);
		}
		for (auto& c : n->children) {
			FillZNodes(zns, c);
		}
	}

	void OrderByZDrawAndClear(List<ZNode>& zns) {
		std::sort(zns.buf, zns.buf + zns.len, ZNode::LessThanComparer);	// draw small z first
		for (auto& zn : zns) {
			zn->Draw();
		}
		zns.Clear();
	}

}
