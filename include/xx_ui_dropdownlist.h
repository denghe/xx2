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
		Weak<Label> lbl;
		TinyFrame icon, itemHead;
		XY totalSize{};
		int32_t selectedIndex{};
		Ref<Scale9Config> cfgBG;
		List<std::string> items;
		Weak<Background> itemsBG;
		Weak<Scale9> itemsBorder;
		Weak<Node> itemsContent;
		std::function<void(int32_t)> onSelectedIndexChanged = [](int32_t idx) { CoutN("DropDownList selectedIndex = ", idx); };

		// init step 1/2
		DropDownList& InitBegin(int32_t z_, XY position_, XY anchor_, XY fixedSize_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_, Ref<Scale9Config> cfgBG_
			, TinyFrame icon_, TinyFrame itemHead_) {
			typeId = cTypeId;
			isFocus = false;
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
		void InitEnd(int32_t selectedIndex_) {
			selectedIndex = selectedIndex_;
			totalSize = { size.x, size.y * items.len };
			auto& cfg = GetCfg();

			auto lblLeft = MakeChildren<Label>();
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, {}, cfg.txtScale, cfg.txtColor);
			lblLeft->SetText(items[selectedIndex]);
			lbl = lblLeft;
			
			auto imgSize = size.y * 0.8f;
			auto imgSpacing = (size.y - imgSize) * 0.5f;
			XY imgPos{ size.x - imgSpacing, imgSpacing };
			MakeChildren<Image>()->Init(z + 2, imgPos, { 1, 0 }, imgSize, true, icon);

			MakeChildren<Scale9>()->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);

			onClicked = [w = WeakFromThis(this)] {
				assert(w);
				w->PopList();
			};
		}

		void PopList() {
			itemsBorder = MakeChildren<Scale9>();
			itemsBorder->Init(z + 1000, 0, { 0, 1 }, cfgBG->borderScale, totalSize / cfgBG->borderScale, *cfgBG);
			itemsBorder->inParentArea = false;

			itemsContent = MakeChildren<Node>();
			itemsContent->Init(z + 1001, 0, { 0, 1 }, 1, totalSize);
			itemsContent->inParentArea = false;

			for (int32_t i = 0; i < items.len; ++i) {
				itemsContent->MakeChildren<DropDownListItem>()->Init(z + 1001
					, WeakFromThis(this), i, false);
			}

			itemsBG = MakeChildren<Background>();
			itemsBG->Init(z + 999, itemsContent).onOutsideClicked = [this] {
				assert(itemsBorder);
				assert(itemsBG);
				assert(itemsContent);
				itemsBorder->SwapRemoveFromParent();
				itemsBG->SwapRemoveFromParent();
				itemsContent->SwapRemoveFromParent();
			};
		}

		void ItemCommit(int32_t idx_) {
			assert(lbl);
			itemsBG->onOutsideClicked();	// unsafe
			if (selectedIndex != idx_) {
				selectedIndex = idx_;
				lbl->SetText(items[idx_]);
				onSelectedIndexChanged(idx_);
			}
		}

		void ApplyCfg() override {
			assert(children.len >= 3);
			auto& cfg = GetCfg();
			assert(children[2]->typeId == Scale9::cTypeId);
			auto bg = (Scale9*)children[2].pointer;
			bg->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}
	};


	inline void DropDownListItem::Init(int32_t z_, Weak<DropDownList> owner_, int32_t idx_, bool highLight_) {
		typeId = cTypeId;
		z = z_;
		owner = std::move(owner_);
		idx = idx_;
		highLight = highLight_;
		size = owner->size;
		position = { 0, owner->totalSize.y - size.y * idx_ };
		anchor = { 0, 1 };
		FillTrans();

		auto imgSize = size.y * 0.8f;
		auto imgSpacing = (size.y - imgSize) * 0.5f;
		auto imgColor = idx_ == owner->selectedIndex ? RGBA8_Green : RGBA8_White;
		MakeChildren<Image>()->Init(z, imgSpacing, { 0, 0 }, imgSize, true, owner->itemHead, {}, imgColor);

		auto& cfg = *owner->cfgNormal;
		auto lblLeft = MakeChildren<Label>();
		XY lblPos{ imgSpacing + imgSize + imgSpacing, cfg.txtPaddingRightBottom.y };
		lblLeft->Init(z, lblPos, {}, cfg.txtScale, cfg.txtColor);
		lblLeft->SetText(owner->items[idx_]);
	}

	inline void DropDownListItem::SetHighLight(bool enable) {
		if (highLight == enable) return;
		highLight = enable;
		assert(children[1]->typeId == Label::cTypeId);
		auto lbl = (Label*)children[1].pointer;
		lbl->color = enable ? RGBA8{ 200, 100, 100, 255 } : RGBA8_White;
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
