#pragma once
#include "xx_gamebase.h"
#include "xx_math.h"

namespace xx {

	struct alignas(8) Node {
		static constexpr int32_t cTypeId{};							// need set typeId in Init

		List<Shared<Node>> children;
		Weak<Node> parent;											// fill by MakeChildren
		Weak<Node> scissor;											// fill by scroll view MakeContent

        XX_INLINE SimpleAffineTransform& trans(){ return (SimpleAffineTransform&)worldScale; }
        XY worldScale, worldMinXY;
        XX_INLINE SimpleAffineTransform const& trans() const { return (SimpleAffineTransform&)worldScale; }

		XY position{}, anchor{ 0.5, 0.5 }, scale{ 1, 1 }, size{};
		XY worldMaxXY{}, worldSize{};								// boundingBox. world coordinate. fill by FillTrans()

		int32_t typeId{};											// fill by Init
		int z{};													// global z for event priority or batch combine
		bool inParentArea{ true };									// panel true ? combo box pop false ?
		float alpha{ 1 };	// todo
		int32_t indexAtParentChildren{-1};
		//uint64_t ud{};

		XX_INLINE XY GetScaledSize() const {
			return scale * size;
		}

		// for init
		XX_INLINE void FillTrans() {
			if (parent) {
				trans() = SimpleAffineTransform::MakePosScaleAnchorSize(position, scale, anchor * size).MakeConcat(parent->trans());
			} else {
				trans().PosScaleAnchorSize(position, scale, anchor * size);
			}

			worldMaxXY = trans()(size);
			worldSize = worldMaxXY - trans().Offset();

			TransUpdate();
		}

		// for draw FillZNodes
		XX_INLINE bool IsVisible() const {
			if (scissor && !IsIntersect_BoxBoxF(worldMinXY, worldMaxXY, scissor->worldMinXY, scissor->worldMaxXY)) return false;
			if (inParentArea && parent) return IsIntersect_BoxBoxF(worldMinXY, worldMaxXY, parent->worldMinXY, parent->worldMaxXY);
			return IsIntersect_BoxBoxF(worldMinXY, worldMaxXY, GameBase::instance->worldMinXY, GameBase::instance->worldMaxXY);
		}

		XX_INLINE bool PosInArea(XY const& pos) const {
			if (scissor && !IsIntersect_BoxPointF(scissor->worldMinXY, scissor->worldMaxXY, pos)) return false;
			return IsIntersect_BoxPointF(worldMinXY, worldMaxXY, pos);
		}

		XX_INLINE bool PosInScissor(XY const& pos) const {
			if (!scissor) return true;
			return IsIntersect_BoxPointF(scissor->worldMinXY, scissor->worldMaxXY, pos);
		}

		// for draw bg
		XX_INLINE XY CalcBorderSize(XY const& padding = {}) const {
			return size * scale + padding;
		}

		// for update
		void FillTransRecursive() {
			FillTrans();
			for (auto& c : children) {
				c->FillTransRecursive();
			}
		};

		void SetAlphaRecursive(float alpha_) {
			alpha = alpha_;
			for (auto& c : children) {
				c->SetAlphaRecursive(alpha_);
			}
		}

		void SetScissorRecursive(Weak<Node> scissor_) {
			scissor = scissor_;
			for (auto& c : children) {
				c->SetScissorRecursive(scissor_);
			}
		}

		// for copy: Node::Init(z_, position_, scale_, anchor_, size_);
		XX_INLINE Node& Init(int z_ = 0, XY const& position_ = {}, XY const& anchor_ = {}, XY const& scale_ = { 1,1 }, XY const& size_ = {}) {
			typeId = cTypeId;
			z = z_;
			position = position_;
			anchor = anchor_;
			scale = scale_;
			size = size_;
			FillTrans();
			return *this;
		}

		// for ui root node only
		XX_INLINE Node& InitRoot(XY const& scale_) {
			return Init(0, 0, 0, scale_);
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
		XX_INLINE Shared<T>& AddChildren(Shared<T> c) {
			assert(c);
			assert(!c->parent);
			c->parent = WeakFromThis(this);
			c->indexAtParentChildren = children.len;
			c->scissor = scissor;
			c->inParentArea = !size.IsZeroSimple();
			return (Shared<T>&)children.Emplace(std::move(c));
		}

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
		XX_INLINE Shared<T>& MakeChildren() {
			return AddChildren(MakeShared<T>());
		}

		void SwapRemoveFromParent() {
			assert(parent);
			assert(parent->children[indexAtParentChildren].pointer == this);
			auto i = parent->children.Back()->indexAtParentChildren = indexAtParentChildren;
			indexAtParentChildren = -1;
			auto p = parent.GetPointer();
			parent.Reset();
			p->children.SwapRemoveAt(i);
		}

		XX_INLINE void Clear() {
			for (auto i = children.len - 1; i >= 0; --i) {
				children[i]->SwapRemoveFromParent();
			}
		}

		virtual void TransUpdate() {};
		virtual void Draw() {};									// draw current node only ( do not contain children )
		virtual ~Node() {};


		void Resize(XY const& scale_) {
			scale = scale_;
			FillTransRecursive();
		}

		void Resize(XY const& position_, XY const& scale_) {
			position = position_;
			scale = scale_;
			FillTransRecursive();
		}

	};

	/**********************************************************************************************/
	/**********************************************************************************************/

	struct ZNode {
		decltype(Node::z) z;
		Node* n;
		XX_INLINE Node* operator->() { return n; }
		XX_INLINE static bool LessThanComparer(ZNode const& a, ZNode const& b) {
			return a.z < b.z;
		}
		XX_INLINE static bool GreaterThanComparer(ZNode const& a, ZNode const& b) {
			return a.z > b.z;
		}
	};

	template<bool skipScissorContent = true>
	inline void FillZNodes(List<ZNode>& zns, Node* n) {
		assert(n);
		if constexpr (skipScissorContent) {
			if (n->scissor && n->scissor == n->parent) return;
		}
		if ((n->size.x > 0.f || n->size.y > 0.f) && n->IsVisible()) {
			zns.Emplace(n->z, n);
		}
		for (auto& c : n->children) {
			FillZNodes(zns, c);
		}
	}

	inline void OrderByZDrawAndClear(List<ZNode>& zns) {
		std::sort(zns.buf, zns.buf + zns.len, ZNode::LessThanComparer);	// draw small z first
		for (auto& zn : zns) {
			zn->Draw();
		}
		zns.Clear();
	}

	template<typename T>
	struct StringFuncs<T, std::enable_if_t<std::is_base_of_v<Node, T>>> {
		static inline void Append(std::string& s, Node const& in) {
			::xx::Append(s, "{ trans = ", in.trans()
				, ", position = ", in.position
				, ", scale = ", in.scale
				, ", anchor = ", in.anchor
				, ", size = ", in.size
				, " }"
			);
		}
	};

	struct MouseEventHandlerNode : Node {
		int32_t indexAtUiGrid{ -1 };

		virtual void OnMouseDown() {};
		virtual void OnMouseMove() {};
		virtual void OnMouseUp() {};

		XX_INLINE bool MousePosInArea() const {		// virtual ?
			return PosInArea(GameBase::instance->mousePos);
		}

		virtual void TransUpdate() override {
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
				g.Remove(indexAtUiGrid);
				indexAtUiGrid = -1;
			}
		}

		~MouseEventHandlerNode() {
			if (indexAtUiGrid != -1) {
				GameBase::instance->uiGrid.Remove(indexAtUiGrid);
				indexAtUiGrid = -1;
			}
		}
	};
}
