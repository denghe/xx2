#pragma once
#include "xx_ui_background.h"
#include "xx_ui_imagebutton.h"

namespace xx {

	struct DropDownList;
	struct DropDownListItem : MouseEventHandlerNode {
		static constexpr int32_t cTypeId{ 15 };
		Weak<DropDownList> owner;
		int32_t idx{ -1 };
		bool highLight{};

		void Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_);

		void SetHighLight(bool enable);

		virtual int32_t OnMouseDown(int32_t btnId_) override;

		virtual int32_t OnMouseMove() override;
	};

	struct DropDownList : Button {
		static constexpr int32_t cTypeId{ 15 };
		Weak<Label> label;
		TinyFrame icon, itemHead;
		XY totalSize{};
		Ref<Scale9Config> cfgBG;	// for popup
		List<std::u32string> items;
		int32_t selectedIndex{};
		std::function<void(int32_t)> onSelectedIndexChanged = [](int32_t idx) { CoutN("DropDownList selectedIndex = ", idx); };

		// init step 1/2
		DropDownList& InitBegin(int32_t z_, XY position_, XY anchor_, XY fixedSize_
			, Ref<Scale9Config> cfgNormal_ = GameBase_ui::GetInstance()->defaultCfg.s9bN
			, Ref<Scale9Config> cfgHighlight_ = GameBase_ui::GetInstance()->defaultCfg.s9bH
			, Ref<Scale9Config> cfgBG_ = GameBase_ui::GetInstance()->defaultCfg.s9bg
			, TinyFrame icon_ = GameBase_ui::GetInstance()->defaultRes.ui_dropdownlist_icon
			, TinyFrame itemHead_ = GameBase_ui::GetInstance()->defaultRes.ui_dropdownlist_head) {
			assert(typeId == cTypeId);
			focused = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			cfgBG = std::move(cfgBG_);
			icon = std::move(icon_);
			itemHead = std::move(itemHead_);
			FillTrans();
			return *this;
		}

		// init step 2/2
		// need items.Add(...
		void InitEnd(int32_t selectedIndex_ = 0) {
			selectedIndex = selectedIndex_;
			totalSize = { size.x, size.y * items.len };
			auto& cfg = GetCfg();
			auto fontSize = size.y - cfg->paddings.TopBottom();
			label = MakeChildren<Label>();
			label->Init(z + 1, cfg->paddings.LeftBottom(), 0, fontSize)(items[selectedIndex]);
			
			XY imgSize{ size.y - cfg->paddings.TopBottom() };
			XY imgPos{ size.x - cfg->paddings.right, cfg->paddings.bottom };
			MakeChildren<Image>()->Init(z + 2, imgPos, {1, 0}, imgSize, true, icon);

			MakeChildren<Scale9>()->Init(z, 0, 0, size, cfg);

			onClicked = [w = WeakFromThis(this)] {
				assert(w);
				w->PopList();
			};
		}

		void SetSelectedIndex(int32_t selectedIndex_) {
			selectedIndex = selectedIndex_;
			label->SetText(items[selectedIndex]);
		}

		void PopList() {
			auto itemsBorder = MakeChildren<Scale9>();
			itemsBorder->Init(z + 1000, 0, { 0, 1 }, totalSize, cfgBG);
			itemsBorder->inParentArea = false;

			auto itemsContent = MakeChildren<Node>();
			itemsContent->Init(z + 1001, 0, { 0, 1 }, 1, totalSize);
			itemsContent->inParentArea = false;

			for (int32_t i = 0; i < items.len; ++i) {
				itemsContent->MakeChildren<DropDownListItem>()->Init(z + 1001
					, WeakFromThis(this), i, false);
			}

			auto itemsBG = MakeChildren<Background>();
			itemsBG->Init(z + 999, itemsContent).onOutsideClicked = [this] {
				children.Resize(3);
			};
		}

		void ItemCommit(int32_t idx_) {
			At<Background>(5).onOutsideClicked();	// unsafe
			if (selectedIndex != idx_) {
				selectedIndex = idx_;
				label->SetText(items[idx_]);
				onSelectedIndexChanged(idx_);
			}
		}

		void ApplyCfg() override {
			At<Scale9>(2).Init(z, 0, 0, size, GetCfg());
		}
	};

	inline void DropDownListItem::Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_) {
		assert(typeId == cTypeId);
		z = z_;
		owner = std::move(owner_);
		idx = idx_;
		highLight = highLight_;
		size = owner->size;
		position = { 0, owner->totalSize.y - size.y * idx_ };
		anchor = { 0, 1 };
		FillTrans();

		auto& cfg = owner->GetCfg();
		auto imgSize = XY{ size.y - cfg->paddings.TopBottom() };
		auto imgColor = idx_ == owner->selectedIndex ? RGBA8_Green : RGBA8_White;
		MakeChildren<Image>()->Init(z, cfg->paddings.LeftBottom(), 0, imgSize, true, owner->itemHead, {}, imgColor);

		MakeChildren<Label>()->Init(z, cfg->paddings.LeftBottom() + XY{ imgSize.x, 0 }, 0, size.y - cfg->paddings.TopBottom())(owner->items[idx_]);
	}

	inline void DropDownListItem::SetHighLight(bool b) {
		if (highLight == b) return;
		highLight = b;
		auto& label = At<Label>(1);
		if (b) label.color = RGBA8{ 200, 100, 100, 255 };
		else label.color = RGBA8_White;
	}

	inline int32_t DropDownListItem::OnMouseDown(int32_t btnId_) {
		assert(owner);
		owner->ItemCommit(idx);
		return 1;
	}

	inline int32_t DropDownListItem::OnMouseMove() {
		if (!highLight) {
			SetHighLight(true);
			GameBase::instance->delayFuncs.Emplace([w = WeakFromThis(this)] {
				auto p = w.TryGetPointer();
				if (!p) return 1;
				if (!p->MousePosInArea()) {
					p->SetHighLight(false);
					return 1;
				}
				return 0;
				});
		}
		return 1;
	}

}
