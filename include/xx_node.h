#pragma once
#include "xx_list.h"
#include "xx_ptr.h"
#include "xx_affine.h"
#include "xx_frame.h"

namespace xx {

    // for rich label
    enum class VAligns : uint8_t {
        Top,
        Center,
        Bottom
    };

    enum class HAligns : uint8_t {
        Left,
        Center,
        Right
    };


    // for ui config
    struct Scale9Config {
        TinyFrame frame;
        XY textureScale{ 3, 3 };
        UVRect center{ 2, 2, 2, 2 };
        RGBA8 color{ RGBA8_White };
        Paddings paddings{ 20, 40, 20, 40 };
    };

    struct alignas(8) Node {
        static constexpr int32_t cTypeId{};							// need set typeId in Init

        List<Shared<Node>> children;
        Weak<Node> parent;											// fill by Make
        Weak<Node> scissor;											// fill by scroll view MakeContent

        SimpleAffineTransform& trans() { return (SimpleAffineTransform&)worldScale; }
        XY worldScale, worldMinXY;
        SimpleAffineTransform const& trans() const { return (SimpleAffineTransform&)worldScale; }

        XY position{}, anchor{ 0.5, 0.5 }, scale{ 1, 1 }, size{};
        XY worldMaxXY{}, worldSize{};								// boundingBox. world coordinate. fill by FillTrans()

        int32_t typeId{};											// fill by Make( need fill it by other makers )
        int32_t indexAtParentChildren{ -1 };							// children[idx] == this
        int32_t z{};												// global z for event priority or batch combine
        float alpha{ 1 };											// for some logic & draw
        bool inParentArea{ true };									// for child cut check. dropdownlist = false
        bool visible{ true };										// false: do not draw
        bool enabled{ true };										// false: do not callback & colorplus = 0.5f
        bool selected{ false };										// draw: always highlight
        bool focused{ false };										// true: highlight
        bool escHandler{ false };									// true for FindTopESCHandler
        VAligns valign{ VAligns::Center };							// only for ui_layouter
        // ...

        XY ToLocalPos(XY worldPos_);
        XY ToParentLocalPos(XY worldPos_);
        XY GetScaledSize() const;

        // for init
        void FillTrans();

        // for draw FillZNodes
        bool IsVisible() const;
        bool PosInArea(XY pos_) const;
        bool PosInScissor(XY pos_) const;

        // for update
        void FillTransRecursive();
        void SetVisibleRecursive(bool visible_);
        void SetEnabledRecursive(bool enabled_);
        void SetAlphaRecursive(float alpha_);
        void SetScissorRecursive(Weak<Node> scissor_);

        template<typename Derived>
        Derived& InitDerived(int32_t z_ = 0, XY position_ = {}, XY anchor_ = {}, XY scale_ = { 1,1 }, XY size_ = {}) {
            assert(typeId == Derived::cTypeId);
            z = z_;
            position = position_;
            anchor = anchor_;
            scale = scale_;
            size = size_;
            FillTrans();
            return (Derived&)*this;
        }

        Node& Init(int32_t z_ = 0, XY position_ = {}, XY anchor_ = {}, XY scale_ = { 1,1 }, XY size_ = {});

        // for ui root node only
        Node& InitRoot(XY scale_);

        // unsafe: return value will failure when children resize if use "auto& rtv = 
        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        Shared<T>& Add(Shared<T> c) {
            assert(c);
            assert(!c->parent);
            c->typeId = T::cTypeId;
            c->parent = WeakFromThis(this);
            c->indexAtParentChildren = children.len;
            c->scissor = scissor;
            c->inParentArea = !size.IsZeroSimple();
            return (Shared<T>&)children.Emplace(std::move(c));
        }

        // unsafe: return value will failure when children resize if use "auto& rtv = 
        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        Shared<T>& Make() {
            return Add(MakeShared<T>());
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Node, T>>>
        T& At(int32_t idx_) {
            assert(children[idx_]->typeId == T::cTypeId);
            return *(T*)children[idx_].pointer;
        }

        void SetToUIHandler(bool handle);
        void SwapRemove();
        void Clear();
        void FindTopESCHandler(Node * &out, int32_t & minZ);

        // recursive find TOP ESC handler
        // null: not found
        Node* FindTopESCHandler();
        virtual void HandleESC();								// maybe need override
        void TryRegisterAutoUpdate();

        virtual int32_t Update() { return 0; };					// return !0 mean unregister auto update
        virtual void TransUpdate() {};
        virtual void Draw() {};									// draw current node only ( do not contain children )
        virtual ~Node() {};

        void Resize(XY scale_);
        void Resize(XY position_, XY scale_);
        Node* FindFirstTypeId(int32_t typeId_);

        template<typename T>
        T* FindFirstType() {
            return (T*)FindFirstTypeId(T::cTypeId);
        }
    };


    struct MouseEventHandlerNode : Node {
        int32_t indexAtUiGrid{ -1 };

        virtual int32_t OnMouseDown(int32_t btnId_) { return 0; };	// return 1: swallow
        virtual int32_t OnMouseMove() { return 0; };
        bool MousePosInArea() const;
        virtual void TransUpdate() override;
        ~MouseEventHandlerNode();
    };

    struct ZNode {
        decltype(Node::z) z;
        Node* n;
        Node* operator->() { return n; }
        inline static bool LessThanComparer(ZNode const& a, ZNode const& b) {
            return a.z < b.z;
        }
        inline static bool GreaterThanComparer(ZNode const& a, ZNode const& b) {
            return a.z > b.z;
        }
    };

    void FillZNodes(List<ZNode>& zns, Node* n, bool skipScissorContent = true);

    void OrderByZDrawAndClear(List<ZNode>& zns);

}
